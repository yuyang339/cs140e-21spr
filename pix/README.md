This is a slightly modified version of fake-user and fake-os from 
lab `10-processes`.

It should be the case that if you drop in your assembly from that
lab, that the checks in `tests` pass.

  - `mk`: different makefile templates.  These will probably need to be 
    changed in the future.
  - `pix-kernel`: a stripped down kernel based off of `fake-os`.
  - `pix-user`: simple user level code: more or less just wrappers
     for system calls and a tiny amount of libc code.
  - `tests`: simple programs to test various things.

steps:
  0. "make check" should work.
  1. replace our .o's (in STAFF_OBJS) and make sure make check still works.
  2. add the process structure and rewrite the equiv code:
     make sure you get the same checksum.
  3. change pix to run multiple times: create a number of processes 
     (they are the same) and make sure get the same checksum.
        should we add an expected checksum?  this makes it easier.
  4. add virtual memory: make sure you get the same checksum.
  5. switch b/n each different process: make sure you get the same
     checksum at the end.

add fork, exec, and waitpid:
    - make sure that you get the same checksum.
        * need to make sure to add this in such a way that it doesn't
          invalidate the other checksums...

other same checksum:
  - add interrupts: make sure you get the same checksum.
  - optimize virtual memory switching.
  - change compiler optimization in pix
  - change to use hashing --- icache and etc.

not enough time to write this.

pid can be a hash of the program text + the fork count of the current
process.

    - however, if you have two of them, are going to get multiple of the 
      same.  need the depth and count from the root?
            or just have a virtual pid?

        can just have a count for it?  or prevent?   or the count from 
        the start of time and keep it in the program?  the basic issue
        is you don't want the same process.

            or just have a map.  can return a hash of the different pids
            decended from the parent?

        aiya.
