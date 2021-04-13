Main rule:
                        DO NOT MODIFY THE MAKEFILE!
                        DO NOT MODIFY THE MAKEFILE!
                        DO NOT MODIFY THE MAKEFILE!
                        DO NOT MODIFY THE MAKEFILE!
                        DO NOT MODIFY THE MAKEFILE!
                        DO NOT MODIFY THE MAKEFILE!


Basic idea: to add code, modify only three places:
  1.  Put your `.c` or `.S` file in `src/`.
  2.  Add the path to the source in `put-your-src-here.mk` so that the
      the `Makefile` will compile them.
  3. If you want to add extra prototypes there is a `libc-extra.h` and an
     `rpi-extra.h` which we will not touch, so you can change freely.

Please do not:
  1. Modify the `Makefile` directly!  You'll get conflicts.
  2. Add files to `staff-src/` or modify our `.h` files.
     Files we add later may conflict with yours.

The rest of the code:
  1. `include`: All the pi specific .h's.
  2. `staff-src/`: All the staff provided r/pi source.   You can write
     your own, but these are not super interesting.
  3. `libc`: `.c` implementation of standard library functions (often
     ripped off from `uClibc`).
  4. `staff-objs`: these are `.o` files we provide. You can always write
     your own and swap them by modifying `./put-your-src-here.mk`.
  5. `objs`: this is where all the .o's get put during make.  you can ignore it.

