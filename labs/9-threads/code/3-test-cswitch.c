/*
 * second test: simple context switching test.
 */

#include "rpi.h"
#include "rpi-thread.h"

void cswitch(uint32_t *old, uint32_t *new);

void notmain() {
    uart_init();
    kmalloc_init();


    /*
     * part 1: check that we can save/restore the same state.
     */
    int i;
    printk("\n\n-----------------------------------------\n");
    printk("Part1: about to cswitch(old,old)\n");
    uint32_t save = 0;
    cswitch(save,save);
    printk("worked: gonna do alot of cswitches\n");
    for(i = 0; i < 100; i++)
        cswitch(save,save);
    printk("Part1: SUCCESS\n");

#if 0
    // fixing this.

    /*
     * Part 2: kinda brain-twisting, we use <cswitch> to checkpoint
     * a set of register values.  We roll back to that execution point
     * by restoring to this checkpoint at a later date.  This should
     * "just work" if your code works.
     */
    printk("\n\n-----------------------------------------\n");
    printk("Part 2: gonna do time travel\n");
    // keep it out of a register
    volatile int *count = kmalloc(sizeof *count); 
    *count = 0;

    uint32_t *ckpt = 0;
    
    printk("before cswitch(ckpt)\n");
    cswitch(ckpt,ckpt);
    printk("after cswitch(ckpt)\n");

    *count += 1;
    if(*count >= 4) {
        printk("done!  count=%d\n", *count);
        assert(*count == 4);
        printk("Part2: SUCCESS!\n");
        clean_reboot();
    }
    printk("going to time travel: cnt = %d\n", *count);
    cswitch(save, ckpt);
#endif

    panic("IMPOSSIBLE: should not reach this\n");
	clean_reboot();
}


void print_and_die(void) { panic("should not call\n"); }

