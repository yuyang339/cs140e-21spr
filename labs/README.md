# The labs

This is only a rough guide ---- we are currently doing significant
lab rewrites.  The first chunk of labs should remain roughly the
same, just with more photos (and maybe video if I can figure out OBS).
The subsequent labs will likely get modified with some new ideas we have
from teaching cs240lx and cs340lx last year.

---------------------------------------------------------------------
### Part 1: Going down.

The first chunk of the class will be writing all the low-level code needed
to run the pi and using modern techniques to validate it.  Doing so will
remove all magic from what is going on since all of the interesting code
on both the pi and unix side will be written by you:

  - [0-blink](0-blink/): get everyone up to speed and all
      necessary software installed.  This should be a fast lab.

  - [1-gpio](1-gpio/): start getting used to understanding hardware
     datasheets by writing your own code to control the r/pi `GPIO` pins
     using the Broadcom document GPIO description.  You will use this
     to implement your own blink and simple network between your r/pi's.

  - [2-cross-check](2-cross-check/): you will use read-write logging
     to verify that your GPIO code is equivalent to everyone else's.
     If one person got the code right, everyone will have it right.

     A key part of this class is having you write all the low-level,
     fundamental code your OS will need.  The good thing about this
     approach is that there is no magic.  A bad thing is that a single
     mistake makes more a miserable quarter.  Thus, we show you modern
     tricks for ensuring your code is correct.

  - [3-bootloader](3-bootloader/): you will implement your own
     bootloader to transfer the code from your laptop to the pi.  The
     most common r/pi bootloader out there uses the `xmodem` protocol.
     This approach is overly complicated.  You will do a much simpler,
     more likely to be correct protocol.  It has the advantage that it
     will later make it easier to do a network boot.

  - [4-uart](4-uart/): you write your first real device driver,
     for the UART, using only the Broadcom document.  At this point,
     all key code on the pi is written by you.  You will use the cross
     checking code from lab3 to verify your implementation matches
     everyone else's.

     A great way to understand the UART hardware is to write your own
     software version of it and use this to communicate with your laptop.
     Doing so gives you a second source of output, which will turn out
     to be very useful later when we do networking.

  - [5-digital-analyzer](5-digital-analyzer/): you build a fast, accurate
    digital analyzer and test signal generator that have timing error
    in the tens of nanoseconds.  This will teach you more about how the
    machine works, and get you more comfortable looking at machine code.
    At the end you'll have software that appears more accurate than a
    $400 Saleae (at least the ones made in 2020).

    The lab is a great example of how writing all the code yourself, and
    having a simple system makes it very easy to do things in-practice
    impossible on a traditional OS such as Linux or MacOS. 

  - [6-sw-uart](6-sw-uart/): our final low level lab you'll build a software
    "bit-banging" version of the UART protocol used in lab 4.  

    This will open your eyes to all a hardware protocol is (a signal that
    goes to 1 or 0, with some timining constraints), in sharp contrast
    to the complexity of the datasheets that describe the hardware that
    implement them.

    In addition, you'll have the startling surprise of seeing first hand
    that --- despite the wildly different domain --- your test generating
    code from last lab is basically identical to how you transmit bytes
    using the UART protocol and your scope code is basically how you
    receive bytes.

At this point you have written low-level device driver code, a bootloader,
and pretty much replaced all of our code.  You've learned how to crush
it under a barrage of automatic checks and done a simple impleementation
of a fake pi environment that allows you to check correctness of pi code
using your laptop.

If you are interested in taking this further, there is a lab from last
year we skipped (zoom makes some things hard), that you might find worth
doing:

  - [optional lab: replay](https://github.com/dddrrreee/cs140e-20win/tree/master/labs/5-replay).   
    This is a lab from the previous class: in a twist on lab3, you
    will use Unix system calls to interpose between your Unix and
    pi bootloader code, record all reads and writes, and test your
    bootloader implementation by replaying these back, both as seen and
    with systematic corruption.

    This approach comes from the model-checking community, and I believe
    after you implement this lab and test (and fix) your bootloader you
    will be surprised if it breaks later.  (In general, the approach
    we follow here applies well to other network protocols which have
    multi-step protocols and many potential failure modes, difficult to
    test in practice.)

    Using read-write equivalance and simple systematic exploration will
    let you check the code thoroughly enough that you will be surprised
    if the code ever breaks.

We are now going to switch gears to intensively implementing core OS
functionality: interrupts, threads, virtual memory and file systems.

-------------------------------------------------------------------------
### Part 2: Threads and Interrupts, with Tricks:

These are going to change alot. We are getting rid of the ESP and going
with an I2C network device instead (the NRF24L01).  I think we'll pull
a simple virtual memory setup earlier so you can have simple memory
protection.

  - [7-interrupts](7-interrupts/): you will walk through a simple,
    self-contained implementation of pi interrupts (for timer-interrupts),
    kicking each line until you understand what, how, why.  You will
    use these to then implement a version of `gprof` (Unix statistical
    profiler) in about 30 lines.

    Perhaps the thing I love most about this course is that because we
    write all the code ourselves, we aren't constantly fighting some
    large, lumbering OS that can't get out of its own way.  As a result,
    simple ideas only require simple code.  This lab is a great example:
    a simple idea, about twenty minutes of code, an interesting result.
    If we did on Unix could spend weeks or more fighting various corner
    cases and have a result that is much much much slower and, worse,
    in terms of insight.

***We are currently here.***


  - [6-threads](6-threads/): we build a simple, but functional
    threads package.  You will write the code for non-preemptive context
    switching:  Most people don't understand such things so, once again,
    you'll leave lab knowing something many do not.


  - [8-device-interrupts](8-device-interrupts): we setup GPIO interrupts,
    then use this to build networking on the esp8266 wireless device.
    You will modify your previous labs so that you have an interrupt-based
    software UART implementation.  You will then use this to connect
    and control the ESP8266 that we give out in class and ping other pi's.

    Without interrupts, it's difficult to get networking working, since
    our GPIO pins (and our UART options) have limited space and, thus,
    unless our code checks them at exactly the right time, incoming
    messages will vaporize.

  - [10-ESP](10-esp8266/): (note: do not do `9-esp8266` the code is not
   useful.)  You use the $2 esp8266 to add networking.   The initial lab
   just makes the esp work on your laptop using the provided library.
   The homework has you migrate it to your pi so you can communicate /
   control other pi's.  The code depends on having an interrupt-based 
   UART channel.

-------------------------------------------------------------------------
### Part 3: file systems.

  - [11-fuse.A](11-fuse/): In this lab you will use the FUSE file
    system to wrap up your pi as a special file system and mount it on your
    laptop, where you can use standard utilities (and your normal shell)
    to interact with it.  You can then control the pi by reading / writing
    to special files: e.g., echoing a `1` to `pi/reboot` to cause a reboot,
    echoing a program to `/pi/run` to run it.

    This lab is a great example of the power of Unix's simple, powerful
    OO-interface that lets you package a variety of disparete things as
    files, directories, links and interact with them using a uniform set
    of verbs (e.g., `open()-read()-write()-close()`).

  - [12-fuse.B](12-fuse/): We continue fuse since it was a big lab :). 

  - [13-fat32](13-fat32/): The SD and virtual memory are the biggest
    unknowns in our universe, so we'll bang out quick versions of each,
    and then circle back around and make your system more real.

    You will write a simple read-only FAT32 file system that can read from
    your SD card. You will use this to do a very OS-style "hello world":
    use your FAT32 to read `hello-fixed.bin` from last lab from your SD
    card, jump to it and run it.

### Part 4: Virtual Memory

  - [14-vm](14-vm/): You take a working VM system and, using the
    ARM documents, replace its page table manipulation with your own.
    You also add exception handling to catch protection and missing
    translation faults.   The former gives you the tools to start doing
    user-level protection, the latter for demand paging and extending
    a stack as code accesses it.

  - [15-vm-ops](15-vm-ops/): The previous lab defined the main noun in the
    virtual memory universe (the page table); this lab does the main verbs
    used to set up the VM hardware, including how to synchronize hardware,
    translation, and page table state (more subtle than it sounds).
    At the end you should be able to delete all our starter code.

***[HAVE NOT UPDATED THE LABS BELOW]***

#### Part 5: processes

  - [16-processes](16-user-level): today you will make user level
    processes.  This will combine the virtual memory, system calls, and
    threads into a capstone implementation.  You will use this ability to
    your fuse-FS support multiprocessing, so that you can have multiple
    programs running at the same time.

  - Additional labs that we can fit in to make the above more real and more
    integrated.

### Optional labs:


  - virtualization: this lab will show how
    to virtualize hardware.  We will use simple tricks to transparently flip
    how your pi code is compiled so you can run it on Unix, only shipping
    the GPIO reads and writes to a small stub on the pi.  As a result,
    you have full Unix debugging for pi code (address space protection,
    valgrind, etc) while getting complete fidelity in how the pi will behave
    (since we ship the reads and writes to it directly).

  - sonar-int: we take a bit of a fun break,
    and bang out a quick device driver for a simple sonar device. You
    will then get a feel for how interrupts can be used to simplify code
    structure (counter-intuitive!)  by adapting the interrupt code from
    the previous lab to make this code better.

