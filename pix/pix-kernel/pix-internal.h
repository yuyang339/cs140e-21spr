#ifndef __PIX_INTERNAL_H__
#define __PIX_INTERNAL_H__

#include "rpi.h"
#include "syscalls.h"
#include "pix-constants.h"
#include "mmu.h"

/**************************************************************
 * configuration and debug support.
 */

// collect the configuration values in a single structure.
extern struct config {
    unsigned    verbose_p:1,        // print sys_debug calls?
                equiv_p:1,          // print hash?
                use_schedule_p:1,   // call the schedule() routine
                mmu_verbose_p:1,    // print mmu_debug() calls?
                proc_verbose_p:1;   // print proc_debug() calls?
} pix_config;

// different levels of debug output: helps turn things off
// and on.
#define sys_debug(msg...) do {      \
    if(pix_config.verbose_p)        \
        output("SYS:" msg);         \
} while(0)
#define mmu_debug(msg...) do {      \
    if(pix_config.mmu_verbose_p)    \
        output("MMU:" msg);         \
} while(0)
#define proc_debug(msg...) do {      \
    if(pix_config.proc_verbose_p)    \
        output("PROC:" msg);         \
} while(0)


/**************************************************************
 * code for loading programs (given that we don't assume a file system)
 * assumes they have been concatenated to the end of the binary using
 * pitag-linker
 *
 */
struct prog {
    const char *prog_name;
    void (*user_code)(void);
    unsigned nbytes;
    unsigned code_addr;
};

/* 
 * Implementation in staff-loader.c: right now is a gross hack, 
 * we copy it to the end of the binary so that we don't run 
 * into issues w/ malloc.
 */
struct prog program_get(void);

/*************************************************************
 * equivalance and single-steppping support.
 */

// run user_fn in single stepping mode at user level using stack <sp>
// n_print = the first n instructions (for debugging)
int equiv_run_fn(int (*user_fn)(void), uint32_t sp, unsigned n_print);

// assembly routine: run <fn> at user level with stack <sp>
//    XXX: visible here just so we can use it for testing.
int user_mode_run_fn(int (*fn)(void), uint32_t sp);

// context of an equivalance job.
typedef struct {
    uint32_t n_inst,    // total instructions run equiv checking
             pc_hash,   // hash of pc values
             reg_hash,  // hash of register values.
             print_first_n,    // print out the first n pc/reg values for debugging
             in_user_p;     // are we in user code yet?
} pix_equiv_t;




/**********************************************************
 * process structure.
 */

// reserve asid=1 for kernel, asid=2 and above for user.
// perhaps should decouple these, since there are tricks
// that can be used.
enum {
    kernel_asid = 1,
    first_user_asid = 2
};

// reg_save offsets for the different registers.
enum {
    SP_OFF = 13,
    PC_OFF = 15,
    SPSR_OFF = 16,
};

typedef struct first_level_descriptor pix_pt_t;

typedef struct pix_env {
    // register save area: keep it at offset 0 for easy
    // assembly calculations.
    uint32_t reg_save[16 + 1];  // 16 general purpose registers + spsr

    // process id: i think we need a virtual one as well to make user 
    // checksums work.
    uint32_t pid;

    // address space id.  for now we keep it simple by killing off any asid on
    // exit.  can be more clever later.
    uint8_t asid;

    pix_pt_t *pt;       // page table.
    
    // used by the equiv code.
    pix_equiv_t ctx;
} pix_env_t;

// clone <orig> and return a new exact copy.
pix_env_t *env_clone(const pix_env_t *orig);

// delete <e>: currently does not free page mappings or 
// clear the TLB (you should do so).
void env_delete(pix_env_t *e);

// get the next runnable env (used by schedule()).
pix_env_t *env_get_next(void);

// global variable that always points to the currently running
// process.
extern pix_env_t *pix_cur_process;

static inline pix_env_t *pix_get_curproc(void) { 
    return pix_cur_process; 
}
static inline void pix_set_curproc(pix_env_t *c) {
    pix_cur_process = c;
}
static inline unsigned env_pid(pix_env_t *e) {
    return e->pid;
}
static inline unsigned pix_pid(void) {
    return env_pid(pix_get_curproc());
}

// runs <p> in equivalance mode --- saves and restores
// registers to p->reg_save.
int equiv_run_fn_proc(pix_env_t *p, int n_print);


/*****************************************************************
 * 1MB section alloc/free routines.
 */

// free section <secn>
unsigned sec_free(uint32_t secn);
// allocate section <secn>: error if already allocated.
long sec_alloc_exact(uint32_t secn);
// allocate any free 1MB section
long sec_alloc(void);
// extract the section number from <pa> and allocate it.
unsigned sec_alloc_pa(uint32_t pa);
// compute the physical address 
uint32_t sec_to_pa(uint32_t secn);

/*****************************************************************
 * simple virtual memory setup.
 */

// initialize the first init process (page table, etc)
// and enable the MMU: when it returns we are running
// with virtual memory on.
void vm_start(pix_env_t *init_env);


// clone the page table <pt_old>: copy any global mappings over,
// duplicate (allocate + copy) any non-global pages.
void vm_pt_clone(pix_pt_t *pt_new,  const pix_pt_t *pt_old);

/****************************************************************
 * simple process scheduling.
 */

// switch to the next runnable process.
void schedule(void) __attribute__((noreturn));

// low level code to switch processes: load the values in 
// <regs> (r0-r15, spsr) and jump to the result.
// 
// XXX: you implement this.
void switchto_asm(uint32_t *regs) __attribute__((noreturn));



// common equivalance printing.
// will only print if running in equiv mode.
static inline void equiv_print(const char *msg) {
    pix_equiv_t c = pix_get_curproc()->ctx;

    if(!c.n_inst)
        return;
    printk("%s\n", msg);
    if(c.n_inst)
        trace("EQUIV:\tnumber instructions = %d\n", c.n_inst);
    if(c.reg_hash)
        trace("EQUIV:\treg hash = %x\n", c.reg_hash);
    // don't print pc hash if we print reg hash.
    else if(c.pc_hash)
        trace("EQUIV:\tpc hash = %x\n", c.pc_hash);
}

#endif
