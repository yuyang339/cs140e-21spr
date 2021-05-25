#include "nrf-internal.h"

// initial values.  see datasheet.
enum { 
    NRF_CONFIG_RESET = 0b1000,
    NRF_STATUS_RESET = 0b1110,
};

// idk about this: i think just have it be two different drivers.
void notmain(void) {
    printk("about to init\n");
    nrf_gpio_init(20);

    // pretty print the initial state.
    nrf_dump("initial state");

    // example of using the low level interface.
    printk("CONFIG:   %b\n",  nrf_get8(NRF_CONFIG));
    printk("RX_ADDR:  %b\n",  nrf_get8(NRF_EN_RXADDR));
    printk("STATUS:   %b\n",  nrf_get8(NRF_STATUS));

    uint8_t v = nrf_get8(NRF_CONFIG);
    printk("CONFIG:   %b\n",     v);
    assert(v == NRF_CONFIG_RESET);

    v = nrf_get8(NRF_STATUS);
    printk("status:   %b\n",     v);
    assert(v == NRF_STATUS_RESET);

    clean_reboot();
}
