## Prelab for nrf24l01p

### Readings

The NRF datasheet:

  - `docs/nRF24L01P_PS_v1.0.annot.pdf` is an annotated copy of the 
     NRF datasheet.  It's big, so read carefully.   We can skip a bunch
     of it, but the parts we care about we really care about.
  - [CHEATSHEET](CHEATSHEET.md) gives a cheat-sheet of page numbers to 
    crucial parts as well as saying what to concentrate on.

The NRF communicates using the serial peripheral interface (SPI) protocol.
This is common on cheap devices, but has the downside that it needs a lot
of wires.  

   -  [The wiki SPI protocol page](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface) is reasonable.

   - Chapter 10 (starting at page 148) of the Broadcom documents describes
     the hardware support.

   - We give you a driver (`staff-objs/spi.o`), but feel free to write
     your own!  It's a fun project.  As with the UART, it's not hard to
     bit-bang your own version.

## Hardware hookup

As mentioned above, because the NRF uses SPI, there's a lot of wires.
It actually adds a couple of extra ones.  You want to be very careful
hooking things up.  Just to make things annoying, there seems to
be a high manufacturing defect rate on these, and broken NRF can be
indistinguishable from a mis-wired one.

Below is the schematic for the pi and NRF.  Before you hook up anything
***make absolutely sure you only power the nrf using 3.3v***.   5v will
damage them (and in a bad way such that they might appear to mostly work,
rather than just fail completely).

When you run `code/0-test-simple` it should print out a reasonable values.

    NRF:initial state:
    NRF:	CONFIG (1000):	PWR_UP=0, PRIM_RX=0, CRC=0 EN_CRC=1
    NRF:	EN_AA (111111):	NRF:all pipes have auto-ack enabled
    NRF:	EN_RXADDR (11):NRF:	pipes enabled:NRF: pipe0=1 NRF: pipe1=1 NRF:
    NRF:	SETUP_AW:	address=5 bytes
    NRF:	SETUP_RETR (=11):	retran delay=250usec, attempts=3
    NRF:	RF_CH(=10):	2.402Mhz
    NRF:	RF_SETUP(=1111):	bwidth = 2Mbs, power= 0dBm
    NRF:	STATUS(=1110): RX-int=0, TX-int=0, RT-int=0 RXpipe=111 (empty), TX-full=0
    NRF:	TX_ADDR = 0xe7e7e7e7e7
    NRF:	RX_ADDR_P0 = 0xe7e7e7e7e7
    NRF:	RX_PW_P0 (0): 0 byte fixed size packets
    NRF:	RX_ADDR_P1 = 0xc2c2c2c2c2
    NRF:	RX_PW_P1 (0): 0 byte fixed size packets
    NRF:	FIFO_STATUS(=10001): TX_FULL=0, TX_EMPTY=1, RX_FULL=0, RX_EMPTY=1

<table><tr><td>
  <img src="images/nrf-wire.jpg"/>
</td></tr></table>
<table><tr><td>
  <img src="images/nrf-wire-close.jpg"/>
</td></tr></table>

A couple of things:
  1. You're going to need two pi's running at the same time, just like
     the sw-uart lab.   So refresh your memory on how to bootload two
     pi's on your laptop.  

  2. If you're ambitious you could rewriting your `my-install` to handle
     multiple binaries and output.  This will make a major difference.

  3. The nrf will stay up even if you reboot the pi --- so to hard reset
     it you will need to unplug and then plug in your pi.  This is obviously
     a big hassle, especially if your OS sometimes redoes the device bindings.

     For this lab, if you have two microUSB power supplies I highly
     recommend redoing the power setup as follows.  For both pi's,
     disconnect the tty-usb power from them, and instead connect a
     microUSB power adapter to each pi.   Plug both adapters into a power
     switch you can quickly turn them off/on, thereby preserving your OS's
     ttyUSB device connection.  Reset then just takes a flip of a switch.
     Photos below.

<table><tr><td>
  <img src="images/pi-power.jpg"/>
</td></tr></table>
<table><tr><td>
  <img src="images/pi-microUSB.jpg"/>
</td></tr></table>

