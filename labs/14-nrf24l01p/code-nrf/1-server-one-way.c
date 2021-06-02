// fragile one way test to just see that something is coming out.
#include "rpi.h"
#include "nrf.h"
#include "nrf-test.h"

// idk about this: i think just have it be two different drivers.
void notmain(void) {
    unsigned n = NTEST;

    printk("am a server: run me second: going to send %d messages\n", n);
    nrf_init_noacked(nrf_conf_unreliable_mk(), server_addr, 4);
    nrf_dump("no-ack server");

    for(unsigned i = 0; i < n; i++) {
        nrf_send_noack(client_addr, &i, 4);
        delay_ms(DELAY_MS);
        printk("server: sent %d\n", i);
    }
    printk("send: done!\n");
}
