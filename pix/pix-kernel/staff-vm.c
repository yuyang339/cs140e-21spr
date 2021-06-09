#include "pix-internal.h"
#include "libc/bit-support.h"

#define mmu_panic(args...) do { staff_mmu_disable(); panic(args); } while(0)

fld_t * staff_mmu_lookup_section(fld_t *pt, unsigned va);

enum {
    // kernel permission: no access at user mode.
    kernel_perm = 0b01,

    // permission: read/write at user level
    user_perm = 0b11,

    // default non-zero domain we used for everything atm.
    dom_id = 1,
};


// is <perm> legal?
static inline int is_legal_perm(uint32_t perm) {
    return (perm & ~0b11) == 0;
}
// is <addr> within GPIO memory?
static inline int is_device_mem(uint32_t addr) {
    addr = mmu_sec_bits_only(addr);
    return addr >= 0x20000000 
        && addr <= 0x20200000;
}

void data_abort_vector(unsigned lr) {
    // b4-44
    unsigned fault_addr;
    asm volatile("MRC p15, 0, %0, c6, c0, 0" : "=r" (fault_addr));
    panic("pc=%x, fault addr=%x\n", lr, fault_addr);
}

/*************************************************************
 * handle physical addresses using aliasing.
 */
static uint32_t va_to_sec(uint32_t va) {
    return va >> 20;
}

static inline uint32_t phys_to_va(uint32_t pa) {
    assert(pa < MAX_SECS * OneMB);
    return pa + PHYS_OFFSET;
}
static inline void *phys_to_va_ptr(uint32_t pa) {
    return (void*)phys_to_va(pa);
}
void *sec_to_alias(uint32_t sec) {
    assert(sec < MAX_SECS);
    return phys_to_va_ptr(sec << 20);
}

// copy 1MB section <from_sec> to <to_sec>
void sec_copy(uint32_t to_sec, uint32_t from_sec) {
    assert((to_sec << 20) >> 20 == to_sec);
    assert((from_sec << 20) >> 20 == from_sec);

    void *to = phys_to_va_ptr(to_sec << 20);
    void *from = phys_to_va_ptr(from_sec << 20);
    mmu_debug("copying to addr %p  from %p\n", to, from);
    mmu_debug("raw to addr %d  raw from %d\n", to_sec, from_sec);
    mmu_debug("raw to addr %p  raw from %p\n", to_sec<<20, from_sec<<20);

    memcpy(to,from,OneMB);
    assert(memcmp(to,from,OneMB) == 0);
}

// zero out 1MB section <sec>
void sec_zero(uint32_t sec) {
    void *addr = phys_to_va_ptr(sec << 20);
    memset(addr, 0, OneMB);
}

// alias [0, nsegments*OneMB) to [start, start+nsegments*OneMB) with:
// - AP set to <kernel_perm>
// - mark as global.
void 
alias_phys_memory(pix_pt_t *pt, uint32_t start, uint32_t nseg, unsigned perm) {
    assert(nseg <= MAX_SECS);

    for(uint32_t n = 0; n < nseg; n++) {
        uint32_t off = n * OneMB;
        fld_t *pte = staff_mmu_map_section(pt, start+off, off, dom_id);
        pte->AP = perm;
        pte->nG = 0;  // global
    }
}

static void alias_global(fld_t *pt, uint32_t addr, unsigned id, unsigned perm) {
    fld_t *pte = staff_mmu_map_section(pt, addr, addr, id);
    pte->AP = perm;
    pte->nG = 0; // global.
}

// alias physical address <addr> to itself and mark global.
void ident_map_global(fld_t *pt, uint32_t addr, unsigned id, unsigned perm) {
    assert(va_to_sec(addr) < MAX_SECS);
    sec_alloc_pa(addr);
    alias_global(pt, addr, id, perm);
}
// alias device memory to itself.
void ident_map_device(fld_t *pt, uint32_t va, unsigned id, unsigned perm) {
    assert(is_device_mem(va));
    assert(va_to_sec(va) > MAX_SECS);
    alias_global(pt, va, id, perm);
}

// for the moment: identity map the user address <va> and mark private
void ident_map_user(fld_t *pt, uint32_t va, unsigned id, unsigned perm) {
    sec_alloc_pa(va);
    fld_t *pte = staff_mmu_map_section(pt, va, va, id);
    pte->AP = perm;
    pte->nG = 1; // not global.
}

// allocate and map the common global kernel regions: GPIO memory,
// the kernel code, data, various stacks.
//
// also, alias physical memory so we can modify memory in a process
// without switching to it.
void vm_map_kernel(pix_pt_t *pt) {

    // 2. setup mappings

    // map the first MB: shouldn't need more memory than this.
    ident_map_global(pt, 0x0, dom_id, kernel_perm);
    // map the heap (page table, etc): for lab cksums must be at 0x100000.
    ident_map_global(pt, 0x100000,  dom_id, kernel_perm);

    // map the GPIO: make sure these are not cached and not writeback.
    // [how to check this in general?]
    ident_map_device(pt, 0x20000000, dom_id, kernel_perm);
    ident_map_device(pt, 0x20100000, dom_id, kernel_perm);
    ident_map_device(pt, 0x20200000, dom_id, kernel_perm);

    // map the kernel stack
    ident_map_global(pt, STACK_ADDR-OneMB, dom_id, 0b10);

    // if we don't setup the interrupt stack = super bad infinite loop
    ident_map_global(pt, INT_STACK_ADDR-OneMB, dom_id, kernel_perm);

    alias_phys_memory(pt, PHYS_OFFSET, MAX_SECS, kernel_perm);
}

void vm_start(pix_env_t *init_proc) {
    kmalloc_init_set_start(OneMB);

    fld_t *pt = staff_mmu_pt_alloc(4096);
    assert(pt == (void*)0x100000);
    vm_map_kernel(pt);

    init_proc->asid = kernel_asid;
    init_proc->pt = pt;
    uint32_t code = init_proc->reg_save[15];
    uint32_t sp = init_proc->reg_save[13];

    // for the moment do an indentity map
    assert(code ==  (0x400000 + 4));
    assert(sp == 0x7000000);
    code &= ~0b111;
    ident_map_user(init_proc->pt, code, dom_id, user_perm);
    ident_map_user(init_proc->pt, sp - OneMB, dom_id, user_perm);

    staff_mmu_init();
    assert(!mmu_is_enabled());

    // set up permissions for the different domains.
    // atm: we just keep these the same for everything.
    staff_write_domain_access_ctrl(0b01 << dom_id*2);

    // use the sequence on B2-25
    staff_set_procid_ttbr0(0x140e, init_proc->asid, init_proc->pt);

    // note: this routine has to flush I/D cache and TLB, BTB, prefetch buffer.
    staff_mmu_enable();
    assert(mmu_is_enabled());
}

// reuse any global entry; allocate and copy any private entry.
void vm_pt_clone(pix_pt_t *pt_new,  const pix_pt_t *pt_old) {
    for(unsigned sec = 0; sec < MAX_PTE ; sec++) {
        const fld_t *pte = &pt_old[sec];
        fld_t *pte_new = &pt_new[sec];

        *pte_new = *pte;
        if(!pte->tag)
            continue;


        // private: allocate a section and copy.
        if(pte->nG) {
            uint32_t page = sec_alloc();
            mmu_debug("XXX: reallocating index %d at section %d\n", sec, page);
            pte_new->sec_base_addr = page;

            uint32_t s0 = pte->sec_base_addr;
            sec_copy(page, s0);
            assert(memcmp(sec_to_alias(page), sec_to_alias(s0), OneMB) == 0);
        }
    }
}
