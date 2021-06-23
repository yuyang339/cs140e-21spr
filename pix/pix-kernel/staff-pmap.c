/************************************************************
 * dumb alloc and free of 1MB sections.
 *
 * you shouldn't have to modify this.
 */
#include "pix-internal.h"

// very dumb freemap: should add reference counts.
static uint8_t sections[MAX_SECS];

// free <secn>: should change to refcnts, right now this
// only works b/c we don't free anything and don't have
// shared memory.
unsigned sec_free(uint32_t secn) {
    assert(secn < MAX_SECS);
    if(!sections[secn])
        panic("section %d is not mapped!\n");
    return sections[secn] = 0;
}

// allocate section <secn>: error if allocated.
long sec_alloc_exact(uint32_t secn) {
    if(secn >= MAX_SECS)
        panic("section %d too large (MAX=%d)\n", secn, MAX_SECS);

    if(sections[secn]) {
        panic("trying to allocate section %d, but alrady allocated\n", secn);
        return -1;
    }
    mmu_debug("allocated sec %d\n", secn);
    sections[secn]=1;
    return secn;
}

// allocate any section.
long sec_alloc(void) {
    for(uint32_t i = 0; i < MAX_SECS; i++) 
        if(!sections[i]) {
            return sec_alloc_exact(i);
        }
    panic("can't allocate section\n");
    return -1;
}

// convert a section to a physical address.
uint32_t sec_to_pa(uint32_t secn) {
    assert(secn < MAX_SECS);
    return secn << 20;
}

// given a physical address allocate the corresponding section.
unsigned sec_alloc_pa(uint32_t pa) {
    assert(pa % OneMB == 0);
    unsigned secn = pa / OneMB;
    assert(secn < MAX_SECS);
    mmu_debug("allocating pa=%x, sec num = %d\n", pa, secn);
    return sec_to_pa(sec_alloc_exact(secn));
}
