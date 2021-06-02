// fragile one-way test to see that something is coming out.
#include "rpi.h"
#include "nrf.h"
#include "nrf-test.h"

void notmain(void) {
    unsigned n = NTEST;

    printk("server: going to do %d trials\n", n);

    nrf_config_t c = nrf_conf_reliable_mk(RETRAN_ATTEMPTS, RETRAN_DELAY);
    nrf_init_acked(c, server_addr, 4);

    for(unsigned i = 0; i < n; i++) {
        if(nrf_send_ack(client_addr, &i, 4) != 4)
            panic("send failed\n");
        printk("server: sent %d\n", i);

        unsigned x;
        if(nrf_get_data_exact(server_addr, &x, 4) != 4)
            panic("impossible\n");
        printk("server: received %d (expected=%d)\n", x,i);
        assert(x == i);
        delay_ms(DELAY_MS);
    }
    printk("server: done!\n");
}
