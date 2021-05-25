#ifndef __NRF_H__
#define __NRF_H__
// public NRF interface.
//  - see nrf-internal.h for a lot of helper routines.


// print out the NRF configuration by reading it from the hardware.
void nrf_dump(const char *msg);

// initialize the GPIO and SPI interface.
//  - the <ce> = pin used to toggle the NRF.
void nrf_gpio_init(unsigned ce);

#endif
