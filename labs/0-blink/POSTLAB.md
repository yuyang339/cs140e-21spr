### Post-lab

If you finished, congratulations!  It may seem like a small bit of code,
but it's a non-trivial step to have a working system that can actually
generate and bootload pi binaries.  There are so many things that can
go wrong, but they did not, or you solved them.

On that note, if you are still struggling with the lab, below are some of the 
methods to figure out what is going on:

SD card issues:

  - If you're having issues with your SD card not showing up at
    all on your computer (i.e., not showing up in Disk Utility or lsblk)
    and you're using a USB-SD Card adapter (particularly the Anker one),
    make sure you plug in the SD card *after* you connect the adapter. If
    you plug in the card before, the adapter may not see it. If that
    doesn't work, also try using the microSD card directly without a
    micro-to-full SD adapter.

LED won't turn on using 3v:

  - Ideally: you are working with a partner and if their LED worked
    you could quickly try theirs (differential debugging).  If it didn't
    work there is likely a problem with your laptop's USB prot.  If it did
    work there is a problem with your hardware (see below).

    If you don't have a partner, it's worth quickly trying another USB port.

  - Bo K. noticed that the red LEDs in this batch often blow out quickly.  Try
    a different color.

  - Make sure your TTL-usb actually has power: for the ones I've seen
    there should also be an LED on.   In the past, we've had up to 1 in
    10 TTLs arrive defective.  Try swapping another one in (differential
    debugging).

  - Make sure the pi is actually on: for A+ there's a small LED on the
    board that should be on.  It's possible your pi is defective out
    of the box (though surprisingly I don't recall seeing this) ---
    it's more likely you did not connect 5v power or ground correctly.
    Recheck!  If you did, swap a different pi.

  - If your LED does not turn on, or is intermittent there may be a problem with the 
    wires ("jumpers") --- make sure that they are pushed all the way down (you should
    not be able to see the silver part of the pin) also, push them back and forth some.
    The LED might turn on (indicating a loose jumper) or turn off (same).  In either
    case try replacing the jumper and throwing away the old one --- intermittent
    connections are a real pain to debug.

  - If it still does not turn on, try a different 3v connection.  There is a very
    small chance that that one is burned out. 

Running `blink-pin20.bin` from the SD card won't blink:

  - Make sure the microSD is seated all the way.
  - As always, check the wiring --- quickly connect back to 3v to see the
    LED goes on.
  - Make sure you copied `blink-pin20.bin` as `kernel.img` on the SD card.
    If you run `cksum blink-pin20.bin` and `cksum kernel.img` they
    should have the same checksum.  Even if you copied correctly, it's
    possible the OS did not write everything out or a stray electrical
    signal corrupted the SD.
  - There is a small chance pin 20 is dead: there are several binaries that
    blink different pins: try those.
  - If you have one, try a second pi at any one of these steps and use it 
    to delta debug down to the faulty component.   

Bootloading:

  - As Zhouheng S. figured out: the pi zero w's require you enable their UART
    using the `config.txt` file: if you have one of these instead of the A+
    make sure your "enable_uart=1" is uncommented out.

  - You may have to give the path to the TTY-usb device on your laptop.
    See the lab.  On MacOS it seems to be prefixed by "cu.".

  - As above, run `cksum bootloader.bin` and `cksum kernel.img` and make sure
    they give the same value!

  - Try running `part1/hello.bin` first to make it prints "hello" --- I do 
    this immediately if I have any kind of problem, since it shows if the 
    UART is connected, getting signals, etc.

  - Again: make sure all your wires are seated.

  - If you have two pi's try the other.  Start swapping components to 
    narrow down the issue if one works and the other does not.
