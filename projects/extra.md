 - Tests!  This might sound like a low-IQ kind of project, but thinking
   carefully about the assumptions of various pieces of code (e.g.,
   vm) and detecting how they are broken requires both understanding
   and cleverness and is a great way to get a deep understanding how
   things work.

   This project would involve writing an aggressive set of tests that
   could blow up as much of our code as possible.  You could also develop
   a more godzilla set of tracing methods to get a more incisive view
   of what code does (rather than just checking its output at the end).

   These would get bundled into next year for sure.

  - A real bare-metal wireless driver.  This is likely a pretty hard
    project, but there's lots of people in the wild that would love it.

    Similarly, taking the SD card driver (or another) and
    aggressively tuning it should make orders of magnitude difference
    in bandwidth/speed.  This is probably easier than wirelesss by
    10x at least.  It's also very useful for other people out there.
    (E.g., search "raspberry pi bare metal sd driver").

