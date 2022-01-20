#include "rpi.h"

// of all the code/data in a pi binary file.
extern char __heap_start__;

// track if initialized.
static int init_p;

unsigned* heap_ptr;
unsigned* kmalloc_addr;

// this is the minimum alignment: must always
// roundup to at least sizeof(union align)
union align {
        double d;
        void *p;
        void (*fp)(void);
};


// some helpers
static inline uint32_t max_u32(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}
static inline unsigned is_aligned(unsigned x, unsigned n) {
    return (((x) & ((n) - 1)) == 0);
}
static inline unsigned is_aligned_ptr(void *ptr, unsigned n) {
    return is_aligned((unsigned)ptr, n);
}
static inline unsigned is_pow2(unsigned x) {
    return (x & -x) == x;
}
static inline unsigned roundup(unsigned x, unsigned n) {
    assert(is_pow2(n));
    return (x+(n-1)) & (~(n-1));
}


// symbol created by libpi/memmap, placed at the end

/*
 * Return a memory block of at least size <nbytes>
 * Notes:
 *  - There is no free, so is trivial: should be just 
 *    a few lines of code.
 *  - The returned pointer should always be 4-byte aligned.  
 *    Easiest way is to make sure the heap pointer starts 4-byte
 *    and you always round up the number of bytes.  Make sure
 *    you put an assertion in.  
 */
void *kmalloc(unsigned nbytes) {
    assert(nbytes);
    demand(init_p, calling before initialized);
    // unimplemented();
    unsigned size = max_u32(sizeof(align), 4);
    if (!is_aligned_ptr(kmalloc_addr, size)) {
        kmalloc_addr = (unsigned *)roundup((unsigned)kmalloc_addr, size);
    }
    
    heap_ptr = kmalloc_addr+roundup(nbytes, size);
    trace("kmalloc=%p\n", heap_ptr);
    return kmalloc_addr;
}

/*
 * address of returned pointer should be a multiple of
 * alignment. 
 */
void *kmalloc_aligned(unsigned nbytes, unsigned alignment) {
    assert(nbytes);
    demand(init_p, calling before initialized);
    demand(is_pow2(alignment), assuming power of two);
    if(alignment <= 4)
        return kmalloc(nbytes);
    // unimplemented();
    if (!is_aligned_ptr(heap_ptr, alignment)) {
        heap_ptr = (unsigned *)roundup((unsigned)heap_ptr, alignment);
    }
    kmalloc_addr = heap_ptr;
    heap_ptr += roundup(nbytes, alignment);
    return kmalloc_addr;
}

/*
 * One-time initialization, called before kmalloc 
 * to setup heap. 
 *    - should be just a few lines of code.
 *    - sets heap pointer to the location of 
 *      __heap_start__.   print this to make sure
 *      it makes sense!
 */
void kmalloc_init(void) {
    if(init_p)
        return;
    init_p = 1;
    // unimplemented();
    heap_ptr = (unsigned *)roundup((unsigned)&__heap_start__, 4);
    kmalloc_addr = heap_ptr;
}

/*
 * alternative to <kmalloc_init>:  set the start 
 * of the heap to <addr>
 */
void kmalloc_init_set_start(unsigned _addr) {
    demand(!init_p, already initialized);
    init_p = 1;
    // unimplemented();
    heap_ptr = (unsigned *)roundup(_addr, 4);
    kmalloc_addr = heap_ptr;
}


/* 
 * free all allocated memory: reset the heap 
 * pointer back to the beginning.
 */
void kfree_all(void) {
    // unimplemented();
    heap_ptr = (unsigned *)(unsigned)&__heap_start__;
}

// return pointer to the first free byte.
// for the current implementation: the address <addr> of any
// allocated block satisfies: 
//    assert(<addr> < kmalloc_heap_ptr());
// 
void *kmalloc_heap_ptr(void) {
    // unimplemented();
    return kmalloc_addr;
}
