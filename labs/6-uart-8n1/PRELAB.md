## Prelab for 8n1 uart.

This lab is from a cute hack Jonathan Kula did in a previous year as
part of his final project.

Reading:
  - Read the [wikipedia page on UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter)
  on how to transmit a byte `B` at a baud rate B.


we sketch the basic approach below.

----------------------------------------------------------------
### A quick and dirty guide to making 8n1 UART using GPIO

We sketch how to transmit and receive 8-bit bytes.

First, pick a baud rate `R` (bits per second) and compute the cycles per bit:

  1. Given baud rate `R`, compute how many micro-seconds `S` you write
     each bit.  For example, for 115,200, this is: `(1000*1000)/115200
     = 8.68us`.

  2. Since the pi can measure cycles more quickly and accurately than
     it can microseconds convert `S` to cycles per bit.  By default the A+
     runs at `700MHz` so that is `700 * 1000 * 1000` cycles per second
     or about `700x10^6/115200 = 6076` cycles per bit.

Second, initialize the `TX` line at startup to be high (1).

Finally, given the cycles per bit `T` (from above: `T=6076` cycles),
transmit byte B as follows:

  0. Initialize the TX pin to be 1.  This is the default.  It stays at
     1 between transmissions so that the receiver can tell the line
     is idle.

  1. Write a 0 (start) for `T` cycles.  (Writing 0 means we will always
     cause a transition given the TX line's default state of 1; this
     allows the receiver to detect when you start sending.)

  2. Write the value of bit 0 for `T` cycles (0 if it is 0, 1 if it is 1).
  3. Write the value of bit 1 ...
  4. Write the value of bit 2 ...
  5. Write the value of bit 3 ...
  6. Write the value of bit 4 ...
  7. Write the value of bit 5 ...
  8. Write the value of bit 6 ...
  9. Write the value of bit 7 ...
  10. Write a 1 (stop) for at-least `T` cycles (do not cheat this or
      a too-fast second transmission can cause the receiver to miss bits).

If you pause and look at the above list, you will notice it *is almost
completely identical to you test generator code from the last lab,*
with the main difference is that it determines whether to send a 0
or a 1 based on data rather than strictly alternating.  Fantastic!
You already know how to write and measure such code  with low error.
That ability will make this lab much easier.

(As an aside, the fact we can reuse seemingly disparate methods in
seemingly "new" domains is a good example of getting multiple things
from one action, which I always feel is the universe telling us we are
on a good path.)

Reception mirrors the above transmit steps:
 1. Wait for a start bit (0).   
 2. Delay for another `T+T/2` cycles so that we are in the center of
    the period that the first data bit will be transmitted.  (In our
    example: `6076 + 3038 = 9114`).
 4. Bit-wise or the value of the RX line into the bit at position 0.
 5. Delay another T cycles so we are in the center of the transmission
    of the next data bit.
 6. Bit-wise or the value of the RX line into the bit at position 1.
 7. ...
 8. Wait for the stop bit.    (Note: if you return immediately,
    without waiting for any stop bit, then you can get incorrect results:
    if the last data bit was 0 and you try to read another byte while
    it is still being transmitted you will think it is a start bit!
    Ask me how I know.)

Some notes:

   - If you look at this list, you'll realize that because of how the
     pi's GPIO was designed we can set and read the value of many pins
     with a single store or load.  As a result, reading and writing
     multiple pins costs no more than reading or writing a single pin.
     We can use this ability to speed up our protocol by sending and
     receiving as many bits in parallel as there are available pins.

   - As you notice from the protocol, it has no acknowledgements.
     As a result, while it is primarily a *serial* protocol, perhaps
     ironically it can be transparently used as a broadcast protocol:
     the sender does not care if there are one or many recipients because
     it does not have to name them, nor does it have to collect and act
     on O(n) acknowledgements.  I'm hopeful this will allow us to do RF
     broadcasting without modification.

   - A limit of the protocol is that the nodes must rely on the baud rate
     *a priori*.   To be conservative, this baud rate will be slower
     than what they can handle.  As part of this lab you can take a
     stab at designing a protocol that will "self-clock" to the speed
     the recipient can keep up with.

   - As-is, the protocol does not work well for running in an interrupt
     handler where we pick off a bit per interrupt.    (E.g., if the
     last bits of the transmitted byte are 1, then you won't see
     any transitions if you are picking up rising/falling edges.)
     You could use an 8n2.  However, for expediency we typically run
     the entire sequence of steps to get a byte in the interrupt handler
     (yeah, gross).  This strategy means that the faster we can make the
     protocol, the less time you need to stay in the interrupt handler.

Many people are dismissive of a "bit-banging" approach to implementing
a hardware protocols (e.g., UART, I2C, SPI) rather than doing it like
a grown up and using hardware.  They will make vague statements that
bit-banging is slow, costly, error prone.  However, bit-banging has
several nice features, and in many cases they are completely wrong.

  1. It is simple.  You don't have to figure out datasheets and
     write a device driver for the peripheral you are trying to coax
     into doing the right thing: just blast out the 0s and 1s using
     GPIO pins.  As a benefit, you will get a much deeper understanding
     of whatever protocol you are building (e.g., compare your knowledge
     of UART after you finish this lab versus writing a hardware device
     driver for the Broadcom UART).

  2. It can avoid hardware error and limits.  E.g., the PCM output of
     the pi is known to be awful.  We can simply build our own cleaner
     ones.  The I2C implementation has low-power issues: same.

  3. It may be faster!  The pi actually has a pretty beefy CPU
     compared to the processors running in the peripherals.  You may be
     able to run at much higher speeds than they can.  You can certainly
     operate in parallel in ways that they cannot.  It's worth comparing
     your final baud and bandwidth to the maximum you can specify for
     the pi's hardware UART(s).

  4. As a variant of faster: bit banging can mean *we do not have to
     use memory barriers*.  Recall from the Broadcom document: every
     time we read or write different peripherals, we have to do a memory
     barrier to make sure that all previous operations retire before
     continuing, but we do not need to do memory barriers when using
     the same peripheral.  Thus, if we bit bang everything on GPIO and
     never switch, we can completely elide barriers.  Given their cost
     (measure them!) this is fantastic.
