// fragile one-way test to see that something is coming out.
#include "rpi.h"
#include "nrf.h"
#include "nrf-test.h"

void notmain(void) {
    unsigned n = NTEST;

    printk("recv: am a client: going to do %d trials\n", n);
    nrf_config_t c = nrf_conf_reliable_mk(RETRAN_ATTEMPTS, RETRAN_DELAY);
    nrf_init_acked(c, client_addr, 4);
    
    nrf_dump("client");

    for(unsigned i = 0; i < n; i++) {
        uint32_t x;

        if(nrf_get_data_exact(client_addr, &x, 4) != 4)
            panic("impossible\n");

        printk("client: received %d (expected=%d)\n", x,i);
        assert(x == i);
        delay_ms(DELAY_MS);

        printk("client: sending %d\n", i);

        if(nrf_send_ack(server_addr, &i, 4) != 4)
            panic("send failed\n");
        printk("client: sent %d\n", i);
    }
    printk("client: done!\n");
}
