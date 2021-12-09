// the code that includes this file must first

// define:

//   - boot_get8(): get 1 byte

//   - boot_put8(): put 1 byte

//   - boot_has_data(): returns 1 if the device has data.

//

// lab extension: fix the code so that it will not block

// forever if no data arrives in <boot_get32>

#ifndef __GETCODE_H__
#define __GETCODE_H__

/*****************************************************************

* You do not need to modify this first chunk of code.

*/

// note: these <boot_get8> calls will lock up

// the machine if you lose data or it does not show up.

uint32_t boot_get32(void) {
    uint32_t u = boot_get8();
    u |= boot_get8() << 8;
    u |= boot_get8() << 16;
    u |= boot_get8() << 24;
    return u;
}

void boot_put32(uint32_t u) {
    boot_put8((u >> 0)  & 0xff);
    boot_put8((u >> 8)  & 0xff);
    boot_put8((u >> 16) & 0xff);
    boot_put8((u >> 24) & 0xff);
}

// send a string <msg> to the unix side to print.

// message format:

//  [PRINT_STRING, strlen(msg), <msg>]

//

// DANGER DANGER DANGER!!!  Be careful WHEN you call this!

// DANGER DANGER DANGER!!!  Be careful WHEN you call this!

// DANGER DANGER DANGER!!!  Be careful WHEN you call this!

// Why:

//      We do not have interrupts and the UART can only hold 8 bytes

//      before it starts dropping them.   so if you print at the same

//      time the UNIX end can send you data, you will likely lose some,

//      leading to weird bugs.  If you print, safest is to do right

//      after you have completely received a message.

static void boot_putk(const char *msg) {
// lame strlen since we do not have one atm.
    int n;
    for(n = 0; msg[n]; n++);
    boot_put32(PRINT_STRING);
    boot_put32(n);
    for(n = 0; msg[n]; n++)
        boot_put8(msg[n]);
}

#define die(code) do {      \
    boot_put32(code);       \
    return -code;           \
} while(0)

/*****************************************************************

* All the code you write goes below.

*/

// IMPLEMENT this routine.

//

// spins checking if there is data (<boot_has_data>

// returns 1): returns 1 when there is data, or

// 0 if there was a timeout.

//

// you will use timer_get_usec() in libpi.small

//   - look at libpi.small/timer.c for how to correctly

//     wait for <n> microseconds given that the counter

//     can overflow.

static unsigned
has_data_timeout(unsigned timeout) {
    // implement this!
    if(!boot_has_data()) {
        delay_ms(timeout);
        return 0;
    } else {
        return 1;
    }
}

// send a <GET_PROG_INFO> message every 300ms.

static void wait_for_data(unsigned usec_timeout) {
    do {
        boot_put32(GET_PROG_INFO);
    } while(!has_data_timeout(usec_timeout));
}

// IMPLEMENT this routine.
//
// Simple bootloader: put all of your code here.

static inline long get_code(void) {
// 1. keep sending GET_PROG_INFO every 300ms until
// there is data.
    wait_for_data(300);

/****************************************************************

* Add your code below: 2,3,4,5,6

*/

    long addr = 0;
    // 2. expect: [PUT_PROG_INFO, addr, nbytes, cksum]
    //    we echo cksum back in step 4 to help debugging.
    uint32_t op;
    while((op = boot_get32()) != PUT_PROG_INFO) {
        boot_get8();
    }
    addr = boot_get32();
    uint32_t nbytes = boot_get32();
    uint32_t cksum = boot_get32();
    // 3. If the binary will collide with us, abort.
    //    you can assume that code must be below where the booloader code
    //    gap starts.
    // 4. send [GET_CODE, cksum] back.

    boot_put32(GET_CODE);
    boot_put32(cksum);
    // 5. expect: [PUT_CODE, <code>]
    //  read each sent byte and write it starting at
    //  <addr> using PUT8

    while((op = boot_get32()) != PUT_CODE) {
        boot_get8();
    }

    uint8_t* pi_addr = (uint8_t*)addr;
    for(int i = 0; i < nbytes; i++) {
        *(pi_addr+i) = boot_get8();
    }

    // 6. verify the cksum of the copied code.
    uint32_t calculated_cksum = crc32((uint8_t*)addr, nbytes);
    if (calculated_cksum != cksum) return 0;
    // 7. send back a BOOT_SUCCESS!
    boot_putk("Yang: success: Received the program!\n");
    boot_put32(BOOT_SUCCESS);
    return addr;
}

#endif