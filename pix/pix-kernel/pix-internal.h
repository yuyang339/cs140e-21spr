#ifndef __PIX_INTERNAL_H__
#define __PIX_INTERNAL_H__

#include "rpi.h"
#include "syscalls.h"
#include "pix-constants.h"
#include "mmu.h"

// collect the configuration values in a single structure.
extern struct config {
    unsigned verbose_p:1,       // verbose?
             equiv_p:1,         // print hash?
                use_schedule_p:1, // call the schedule() routine
            mmu_verbose_p:1,
            proc_verbose_p:1;
} pix_config;

#define sys_debug(msg...) do { if(pix_config.verbose_p) output(msg); } while(0)
#define mmu_debug(msg...) do {      \
    if(pix_config.mmu_verbose_p)    \
        output("MMU:" msg);         \
} while(0)
#define proc_debug(msg...) do {      \
    if(pix_config.proc_verbose_p)    \
        output("PROC:" msg);         \
} while(0)


// from loader.c: right now is a gross hack,
// we copy it to the end of the binary so that 
// we don't run into issues w/ malloc.
struct prog {
    const char *prog_name;
    void (*user_code)(void);
    unsigned nbytes;
    unsigned code_addr;
};
struct prog program_get(void);

// assembly routine: run <fn> at user level with stack <sp>
//    XXX: visible here just so we can use it for testing.
int user_mode_run_fn(int (*fn)(void), uint32_t sp);

/*******************************************************
 * part 2: single step
 */

// run a single routine <user_fn> in single step mode using stack <sp>
int single_step_run_fn(int (*user_fn)(void), uint32_t sp);

// return the number of instructions we single stepped.
//  (just count the faults)
unsigned single_step_cnt(void);

// assembly routine: run <fn> at user level with stack <sp>
//    XXX: visible here just so we can use it for testing.
int user_mode_run_fn(int (*fn)(void), uint32_t sp);

/*******************************************************
 * part 3/4: pc equiv
 */

// will only print if running in equiv mode.
void equiv_print(const char *msg);

// run user_fn in single stepping mode at user level using stack <sp>
// n_print = the first n instructions (for debugging)
int equiv_run_fn(int (*user_fn)(void), uint32_t sp, unsigned n_print);



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
typedef struct first_level_descriptor pix_pt_t;

// reserve 1 for kernel, 2 and above for user.
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

typedef struct {
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

extern pix_env_t *pix_cur_process;

static inline pix_env_t *pix_get_curproc(void) { 
    return pix_cur_process; 
}
static inline void pix_set_curproc(pix_env_t *c) {
    pix_cur_process = c;
}

void switchto_asm(uint32_t *regs, unsigned spsr) __attribute__((noreturn));
int equiv_run_fn_proc(pix_env_t *p, int n_print);


// used to free and allocate sections.
unsigned sec_free(uint32_t secn);
long sec_alloc_exact(uint32_t secn);
long sec_alloc(void);
unsigned sec_alloc_pa(uint32_t pa);
uint32_t sec_to_pa(uint32_t secn);

#if 0
// make the first init process and start virtual memory
pix_pt_t *vm_init(uint32_t code, uint32_t sp, uint8_t asid);
#endif

// make the first init process and start virtual memory
void vm_start(pix_env_t *init_env);

void schedule(void) __attribute__((noreturn));

pix_env_t *env_clone(pix_env_t *orig);
void env_delete(pix_env_t *e);
pix_env_t *env_get_next(void);
unsigned  env_id(pix_env_t *e);
unsigned pix_pid(void);

void vm_pt_clone(pix_pt_t *pt_new,  const pix_pt_t *pt_old);

#endif
