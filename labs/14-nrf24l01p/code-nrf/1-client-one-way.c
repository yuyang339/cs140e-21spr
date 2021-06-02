// fragile one-way test to see that something is coming out.
#include "rpi.h"
#include "nrf.h"
#include "nrf-test.h"

void notmain(void) {
    unsigned n = NTEST;
    printk("client %x: start me first!  going to do %d trials\n", 
            client_addr, n);

    nrf_init_noacked(nrf_conf_unreliable_mk(), client_addr, 4);
    unsigned ntimeout = 0;

    // 2 second timeout: weird logic, sorry.
    unsigned timeout = 2 * 1000 * 1000;
    for(unsigned i = 0; i < n; i++) {
        uint32_t x;

        // can get an initial timeout, which will be counted.
        if(nrf_get_data_exact_timeout(client_addr, &x, 4, timeout) == 4)
            printk("client: received %d (expected=%d)\n", x,i);
        else {
            output("%d: get failed\n", i);
            ntimeout++;
        } 
    }
    printk("timeouts=%d\n", ntimeout);
    printk("client: done!\n");
}
