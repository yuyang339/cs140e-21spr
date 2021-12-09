/*

* engler, cs140e: simple unix-side bootloader implementation.

* see the lab README.md for the protocol definition.

*

* all your modifications should go here.

*/
// enum {

//     ARMBASE=0x8000, // where program gets linked.  we could send this.

//     // the weird numbers are to try to help with debugging

//     // when you drop a byte, flip them, corrupt one, etc.

//     BOOT_START      = 0xFFFF0000,

//     GET_PROG_INFO   = 0x11112222,       // pi sends

//     PUT_PROG_INFO   = 0x33334444,       // unix sends

//     GET_CODE        = 0x55556666,       // pi sends

//     PUT_CODE        = 0x77778888,       // unix sends

//     BOOT_SUCCESS    = 0x9999AAAA,       // pi sends on success

//     BOOT_ERROR      = 0xBBBBCCCC,       // pi sends on failure.

//     PRINT_STRING    = 0xDDDDEEEE,       // pi sends to print a string.

//     // error codes from the pi to unix

//     BAD_CODE_ADDR   = 0xdeadbeef,

//     BAD_CODE_CKSUM  = 0xfeedface,

// };

// =======================================================

//          pi side             |               unix side

//  -------------------------------------------------------

//   put_uint(GET_PROG_INFO)+ ----->

//                                   put_uint(PUT_PROG_INFO);

//                                   put_uint(ARMBASE);

//                                   put_uint(nbytes);

//                          <------- put_uint(crc32(code));

//   put_uint32(GET_CODE)

//   put_uint32(crc32)      ------->

//                                   <check crc = the crc value sent>

//                                   put_uint(PUT_CODE);

//                                   foreach b in code

//                                        put_byte(b);

//                           <-------

//  <copy code to addr>

//  <check code crc32>

//  put_uint(BOOT_SUCCESS)

//                           ------->

//                                    <done!>

//                                    start echoing any pi output to

//                                    the terminal.

//  =======================================================


#include "libunix.h"
#include "simple-boot.h"

// Implement steps

//    1,2,3,4.

//

//  0 and 5 are implemented as demonstration.

//

// Note: if timeout in <set_tty_to_8n1> is too small (set by our caller)

// you can fail here. when you do a read and the pi doesn't send data quickly enough.

void simple_boot(int fd, const uint8_t *buf, unsigned n) {
    // if you want to trace PUT/GET set
    trace_p = 1;
    if(pi_roundup(n,4) % 4 != 0)
    panic("boot code size (%d bytes) is not a multiple of 4!\n", n);
    // all implementations should have the same message: same bytes,
    // same crc32: cross-check these values to detect if your <read_file>
    // is busted.
    trace("simple_boot: sending %d bytes, crc32=%x\n", n, crc32(buf,n));
    output("waiting for a start\n");
    uint32_t op;

// we drain the initial pipe: can have garbage.  the code is a bit odd b/c

// if we have a single garbage byte, then reading 32-bits will

// will not match <GET_PROG_INFO> (obviously) and if we keep reading

// 32 bits then we will never sync up with the input stream, our hack

// is to read a byte (to try to sync up) and then go back to reading 32-bit

// ops.

//

// CRUCIAL: make sure you use <get_op> for the first word in each

// message.

    while((op = get_op(fd)) != GET_PROG_INFO) {
        output("expected initial GET_PROG_INFO, got <%x>: discarding.\n", op);
        // have to remove just one byte since if not aligned, stays not aligned
        get_uint8(fd);
    }

    // 1. reply to the GET_PROG_INFO
    // unimplemented();
    trace_put32(fd, PUT_PROG_INFO);

    // 2. drain any extra GET_PROG_INFOS

    // unimplemented();

    uint32_t cksum = crc32(buf,n);
    trace_put32(fd, ARMBASE);
    trace_put32(fd, n);
    trace_put32(fd, cksum);
    // 3. check that we received a GET_CODE
    // unimplemented();
    while((op = get_op(fd)) != GET_CODE) {
        output("expected GET_CODE, got <%x>: discarding.\n", op);
        // have to remove just one byte since if not aligned, stays not aligned
        get_uint8(fd);
    }

uint32_t received_cksum = trace_get32(fd);
if (received_cksum != cksum) return;
// 4. handle it: send a PUT_CODE + the code.
// unimplemented();
    trace_put32(fd, PUT_CODE);
    for(int i=0; i<n; i++) {
        trace_put8(fd, *(buf+i));
    }

    // 5. Wait for BOOT_SUCCESS
    ck_eq32(fd, "BOOT_SUCCESS mismatch", BOOT_SUCCESS, get_op(fd));
    output("bootloader: Done.\n");
}