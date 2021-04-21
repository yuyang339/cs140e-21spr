// simple ping-pong between a client and server.
// slow overly verbose implementations to make easier to see what is going on.
//
// make a copy and modify this to have four wires between the two pi's:
//    - two "ready for data", two uart wires.
//    - set "ready for data" to 1 when ready to receive a character.
//    - set "ready for data" to 0 when not ready to receive a character.
//    - require a transition between them.
//
#include "rpi.h"
#include "cycle-count.h"
#include "cycle-util.h"
#include "sw-uart.h"

// 5 second timeout
unsigned timeout = 5 * 1000 * 1000;

static void server(unsigned tx, unsigned rx, unsigned n);

static void server(unsigned tx, unsigned rx, unsigned n) {
    gpio_write(tx,1);

    unsigned x, v = 0;
    unsigned expected = 1;
    printk("am a server\n");
    
    for(unsigned i = 0; i < n; i++) {
        // oh: have to wait.
        if(!wait_until_usec(rx, expected, timeout)) 
            panic("timeout waiting for %d!\n", expected);
        printk("received %d\n", expected);
        expected = 1 - expected;

        printk("%d: going to write: %d\n", i, v);
        gpio_write(tx,v);
        v = 1 - v;
    }
    printk("server done\n");
}

static void client(unsigned tx, unsigned rx, unsigned n) {
    printk("am a client\n");

    // our first send should be 1
    unsigned x, v = 1;
    // we received 1 from server: next should be 0.
    unsigned expected = 0;
    
    for(unsigned i = 0; i < n; i++) {
        printk("%d: going to write: %d\n",i, v);
        gpio_write(tx,v);
        v = 1 - v;

        if(!wait_until_usec(rx, expected, timeout)) 
            panic("timeout, waiting for %d!\n", expected);
        printk("received %d\n", expected);
        expected = 1 - expected;
    }
    printk("client done\n");
}

// send N samples at <ncycle> cycles each in a simple way.
static void ping_pong(unsigned tx, unsigned rx, unsigned n) {
    gpio_set_output(tx);
    gpio_set_output(rx);

    if(!gpio_read(rx))
        server(tx,rx,n);
    else
        client(tx,rx,n);
}

void notmain(void) {
    cycle_cnt_init();
    ping_pong(21,20,128);
    clean_reboot();
}
