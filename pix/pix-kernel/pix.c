// engler: cs140e, 2020.
//
// starter code for pix kernel.  very similar to fake-os in lab-10
//
// extension: virtualize results so that you can still use the hash.
// e.g., have fork() return a virtual pid so the checksums still work
// out.
#include "rpi.h"
#include "rpi-interrupts.h"
#include "vector-base.h"
#include "reg-util.h"
#include "pix-internal.h"
#include "cp14-debug.h"

struct config pix_config = {
    .verbose_p = 0,         // set to 1 for debug output.
    .equiv_p = 1,
    .mmu_verbose_p = 0,     // 1 for virtual mem debug
    .proc_verbose_p = 0,    // 1 for process debug
};

pix_env_t *pix_cur_process;

static const char *prog_name = "hello";

// simple round robin scheduling.
void schedule(void) {
    proc_debug("in schedule\n");
    pix_env_t *e = env_get_next();
    if(!e) {
        output("pix done. rebooting!\n");
        clean_reboot();
    }
    proc_debug("going to run %d\n", env_pid(e));

    staff_set_procid_ttbr0(0x140e, e->asid, (void*)e->pt);
    pix_set_curproc(e);
    proc_debug("switched: going to jump to pid=%d: e->init=%p\n", pix_pid(),
                    e->reg_save[15]);
    switchto_asm(&e->reg_save[0]);
}


uint32_t do_syscall(uint32_t sysnum, uint32_t a0, uint32_t a1, uint32_t a2) {
    // debug("mode=%s, sp=%x\n", mode_str(reg_get_cpsr()),reg_get_sp());

    sys_debug("syscall=%d, a0=%x,a1=%x,a2=%x\n", sysnum, a0,a1,a2);
    switch(sysnum) {
    case SYS_PUT_HEX: 
        if(pix_config.verbose_p) printk("USER: %x\n", a0);  
        else printk("%x", a0);
        return 0;
    case SYS_PUTC: 
        if(pix_config.verbose_p) printk("USER: %c\n", a0);  
        else printk("%c", a0);
        return 0;
    case SYS_EXIT: 
        printk("%s: sys_exit(%d)\n", prog_name, a0);
        if(pix_config.equiv_p)
            equiv_print("equiv values");

        if(!pix_config.use_schedule_p) {
            printk("%s: sys_exit(%d): going to reboot\n", prog_name, a0);
            clean_reboot();
        // if we're supposed to use schedule()
        } else {
            printk("%s: sys_exit(%d): going to call schedule\n", prog_name, a0);
            // if there are more processes, switch.
            pix_env_t *e = pix_get_curproc();
            env_delete(e);
            schedule();
            not_reached();
        }
    default: panic("undefined system call: %d\n", sysnum);
    }
}

pix_env_t pix_env_mk(uint32_t pc, uint32_t sp) {
    pix_env_t p = {0};

    p.reg_save[0] = pc;
    p.reg_save[1] = sp;
    p.reg_save[SPSR_OFF] = 0x190;
    p.reg_save[PC_OFF] = pc;
    p.reg_save[SP_OFF] = sp;
    
    printk("pc=%x, sp=%x\n", pc,sp);
    return p;
}

void notmain(void) {
    extern uint8_t part4_equiv_vec, proc_equiv_vec;

    printk("kernel: stack is roughly at: %x\n", reg_get_sp());

    struct prog p = program_get();
    prog_name = p.prog_name;
    int (*code)(void) = (void*)p.user_code;

    debug("mode=%s, sp=%x\n", mode_str(cpsr_get()),reg_get_sp());

    unsigned part = 0;
    assert(part <= 5);
    // part 0: just make sure things work.
    if(part == 0) {
        // this just uses your lab 10.
        vector_base_set(&part4_equiv_vec);
        equiv_run_fn(code, STACK_ADDR2, 4);
    // the rest:
    //  1. use the process structure.
    //  2. use virtual memory.
    //  3. do multiple runs.
    //  4. run multiple processes
    } else {
        // have to uncomment these as you define the 
        // different routines.
        // don't put on the stack.
        static pix_env_t init_proc;

        // part 1: change equiv to use the process structure.
        init_proc = pix_env_mk((uint32_t)code, STACK_ADDR2);
        pix_set_curproc(&init_proc);
        assert(pix_get_curproc() == &init_proc);

        // this uses the vector routine you made at the 
        // start of the lab.
        vector_base_set(&proc_equiv_vec);

        switch(part) {
        case 1:
            printk("about to do part 1: proc struct\n");
            // this uses just the proc structure
            equiv_run_fn_proc(&init_proc, 4);
            break;
        case 2: 
            // use your code to initialize vm
            printk("about to do part 2: vm\n");
            vm_start(&init_proc);
            equiv_run_fn_proc(&init_proc, 4);
            break;
        // ugly, but there's enough going on that we keep the
        // parts together.
        case 3:
        case 4:
        case 5:
            printk("about to do part %d: vm\n", part);
            
            vm_start(&init_proc);
            printk("about to clone\n");
            pix_env_t *e = env_clone(&init_proc);
            printk("cloned!\n");

            // we only use at exit.
            if(part >= 4)
                pix_config.use_schedule_p = 1;

            if(part >= 5) {
                printk("you'll have to do a `make run` rather than `make check`\n");
                // start off with just one more process
                env_clone(&init_proc);
                // uncomment to add more.
                // env_clone(&init_proc);
                // env_clone(&init_proc);
                // env_clone(&init_proc);
                // env_clone(&init_proc);
            }

            // if this doesn't work, try again with init_proc
            // equiv_run_fn_proc(&init_proc, 4);

            pix_set_curproc(e);
            equiv_run_fn_proc(e, 4);
            break;

        default: 
            panic("invalid part\n");
        }
    }
    not_reached();
}
