/************************************************************
 * staff code for the environment structure.
 *  - brain dead approach to a process queue: just an array.
 *
 * you shouldn't have to modify this.
 */
#include "pix-internal.h"

static pix_env_t envs[MAX_ENV];
static unsigned new_pid = 1;

unsigned  env_id(pix_env_t *e) {
    unsigned id = e - &envs[0];
    assert(id < MAX_ENV);
    return id;
}

unsigned pix_pid(void) {
    pix_env_t *e = pix_get_curproc();
    return e->pid;
}

void env_delete(pix_env_t *e) {
    e->pid = 0;
}

static pix_env_t *env_alloc(void) {
    for(int i = 0; i < MAX_ENV; i++) {
        pix_env_t *e = &envs[i];
        if(e->pid)
            continue;
        e->pid = new_pid++;
        e->asid = first_user_asid + i;
        return e;
    }
    panic("not enough envs\n");
}

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

pix_env_t *env_clone(pix_env_t *e_old) {
    pix_env_t *e_new = env_alloc();
    memcpy(e_new->reg_save, e_old->reg_save, 
        sizeof e_old->reg_save);

    if(!e_new->pt)
        e_new->pt = staff_mmu_pt_alloc(MAX_PTE);
    
    vm_pt_clone(e_new->pt, e_old->pt);

    return e_new;
}
