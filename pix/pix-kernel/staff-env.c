/************************************************************
 * staff code for the environment structure.
 *  - brain dead approach to a process queue: just an array using
 *    a for-loop to search.   
 *
 * you shouldn't have to modify this for the lab: you can certainly
 * rewrite it!
 */
#include "pix-internal.h"

static pix_env_t envs[MAX_ENV];
static unsigned new_pid = 1;

static inline unsigned  env_id(pix_env_t *e) {
    unsigned id = e - &envs[0];
    assert(id < MAX_ENV);
    return id;
}

// XXX: bug in that we do not delete all mappings
// that use the ASID ---  if we re-use the env.
// should have a flush count or boolean so
// you can tell if it needs to be flushed on realloc?
void env_delete(pix_env_t *e) {
    e->pid = 0;
}

// allocate a free env
static pix_env_t *env_alloc(void) {
    for(int i = 0; i < MAX_ENV; i++) {
        pix_env_t *e = &envs[i];
        if(e->pid)
            continue;

        // we don't reset the pt.
        pix_pt_t *pt = e->pt;
        memset(e, 0, sizeof *e);
        e->pt = pt;
        e->pid = new_pid++;
        e->asid = first_user_asid + i;
        return e;
    }
    panic("not enough envs\n");
}

// get the next runnable env: maybe just put these
// in a linked list?  idk.
pix_env_t *env_get_next(void) {
    assert(pix_cur_process);
    unsigned id = env_id(pix_cur_process);

    // round robin.
    for(unsigned i = 0; i < MAX_ENV; i++) {
        unsigned index = (i + id) % MAX_ENV;
        pix_env_t *e = &envs[index];
        if(e->pid) {
            proc_debug("found env=%d\n", index);
            return e;
        }
    }
    output("no more processes\n");
    return 0;
}

// exactly clone <e_old> other than 
// the asid/pid.
pix_env_t *env_clone(const pix_env_t *e_old) {
    pix_env_t *e_new = env_alloc();

    // reg_save is crucial state.
    memcpy(e_new->reg_save, e_old->reg_save, 
        sizeof e_old->reg_save);

    // if we already have a page table allocated
    // from a previous run, reuse it.   it will
    // get completely reset by <vm_pt_clone>
    if(!e_new->pt)
        e_new->pt = staff_mmu_pt_alloc(MAX_PTE);
    
    // if we hash the page table contents by replacing
    // the section number in each entry with a hash of 
    // the 1MB section it points to, both page tables
    // should have the same hash.  use this later for
    // equiv.
    vm_pt_clone(e_new->pt, e_old->pt);

    return e_new;
}
