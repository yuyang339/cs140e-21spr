/* engler, 140e: sorta cleaned up test code from 14-vm.  */
#include "rpi.h"
#include "rpi-constants.h"
#include "rpi-interrupts.h"

#include "mmu.h"

static const unsigned OneMB = 1024 * 1024 ;

static volatile struct proc_state {
    fld_t *pt;
    unsigned sp_lowest_addr;
    unsigned fault_addr;
    unsigned dom_id;
} proc;


// cannot use normal panic/assert b/c mmu on.
#define mmu_panic(args...) do { mmu_disable(); panic(args); } while(0)
#define mmu_assert(cond) do { if(!(cond)) { mmu_disable(); assert(cond); } } while(0)

// 
// 1. grow the stack on a fault 
// 2. reset the permissions if you get a write to a r/o
//
void data_abort_vector(unsigned lr) {
    unsigned fault_addr;
    // b4-44
    asm volatile("MRC p15, 0, %0, c6, c0, 0" : "=r" (fault_addr));

    printk("data_abort fault at %x, expected=%x\n", fault_addr, proc.fault_addr);

    mmu_assert(proc.fault_addr == fault_addr);
    mmu_panic("implement the rest of the abort handler\n");
}

// shouldn't happen: need to fix libpi so we don't have to do this.
void interrupt_vector(unsigned lr) {
    staff_mmu_disable();
    panic("impossible\n");
}

/*  first part of this test is the same.  */ 
void vm_test(void) {

    // 1. init
    staff_mmu_init();
    assert(!mmu_is_enabled());

    void *base = (void*)0x100000;
    fld_t *pt = mmu_pt_alloc_at(base, 4096*4);
    assert(pt == base);

    // we use a non-zero domain id to test things out.
    unsigned dom_id = 1;

    // 2. setup mappings

    // map the first MB: shouldn't need more memory than this.
    staff_mmu_map_section(pt, 0x0, 0x0, dom_id);
    // map the page table: for lab cksums must be at 0x100000.
    staff_mmu_map_section(pt, 0x100000,  0x100000, dom_id);

    // map the GPIO: make sure these are not cached and not writeback.
    // [how to check this in general?]
    staff_mmu_map_section(pt, 0x20000000, 0x20000000, dom_id);
    staff_mmu_map_section(pt, 0x20100000, 0x20100000, dom_id);
    staff_mmu_map_section(pt, 0x20200000, 0x20200000, dom_id);

    // map stack (grows down)
    staff_mmu_map_section(pt, STACK_ADDR-OneMB, STACK_ADDR-OneMB, dom_id);

    // if we don't setup the interrupt stack = super bad infinite loop
    staff_mmu_map_section(pt, INT_STACK_ADDR-OneMB, INT_STACK_ADDR-OneMB, dom_id);
    proc.pt = pt;
    proc.sp_lowest_addr = STACK_ADDR - OneMB;
    proc.dom_id = dom_id;

    // 3. install fault handler to catch if we make mistake.
    mmu_install_handlers();

    // 4. start the context switch:
     
    // set up permissions for the different domains.
    staff_write_domain_access_ctrl(0b01 << dom_id*2);

    // use the sequence on B2-25
    staff_set_procid_ttbr0(0x140e, dom_id, pt);

    // note: this routine has to flush I/D cache and TLB, BTB, prefetch buffer.
    staff_mmu_enable();

    /*********************************************************************
     * 5. trivial test: write a couple of values, make sure they succeed.
     *
     * you should write additional tests!
     */
    assert(mmu_is_enabled());

    // read and write a few values.
    int x = 5, v0, v1;
    assert(get32(&x) == 5);

    v0 = get32(&x);
    printk("doing print with vm ON\n");
    x++;
    v1 = get32(&x);


    /*********************************************************************
     * Implement data fault handler and do two tests:
     *  1. an out-of-bounds stack write: it should grow.
      * 2. do a write with insufficient privleges and reset them.
     */

    // 1: out of bounds stack write.
    // we know this is not mapped.
    volatile uint32_t *p = (void*)(STACK_ADDR - OneMB - 16);
    printk("HASH: about to do an invalid write to %x\n", p);
    proc.fault_addr = (unsigned)p;
    *p = 11;
    assert(*p == 11);
    assert(get32(p) == 11);
    printk("HASH:done with invalid write to %x have=%d\n", p,*p);
    printk("HASH:test1: success\n");

    // 2: do a write with insufficient privleges.
    printk("about to mark no access%x (vpn=%x)\n", p, mmu_sec_bits_only((uint32_t)p));
    uint32_t fa = proc.fault_addr = (unsigned)p;
    mmu_mark_sec_no_access(proc.pt, fa, 1);
    printk("HASH: marked no access%x (vpn=%x)\n", p, mmu_sec_bits_only(fa));
    put32(p, 12);
    assert(*p == 12);
    printk("HASH: done with invalid perm write to %x have=%d\n", p,*p);
    printk("HASH: test2: success!\n");


    /*********************************************************************
     * DONE: with trivial test, re-enable.
     */

    // 6. disable.
    staff_mmu_disable();
    assert(!mmu_is_enabled());
    printk("OFF\n");

    // 7. make sure worked.
    assert(v0 == 5);
    assert(v1 == 6);
    printk("******** success ************\n");
}

void notmain() {
    uart_init();
    printk("implement one at a time.\n");
    check_vm_structs();
    vm_test();
    clean_reboot();
}
