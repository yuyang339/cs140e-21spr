## simple user processes and equivalance checking.

Today you're going to do a wildly useful trick for ruthlessly detecting
subtle operating system mistakes.  It will prove invaluable next week
when we do virtual memory --- a topic that has extremely hard-to-track-down
bugs if you just use "normal" testing of running programs and checking 
print statements and exit codes.

In order to do so we give you:
  1. A trivial operating system (`fake-os`) that has a single process,
     switches to it, and when it is finished, reboots.

  2. A trivial user-level mode implementation has has three system calls:
     `sys_putc`, `sys_put_hex` and `sys_exit`.

  3. The user level code only communicates with the system level using 
     system calls.

This will start getting you used to user-level vs kernel stuff.  Also,
we need code at user level so (1) single-stepping works and (2) the
addresses are the same for everyone.

### What to do

I will fill in a bunch of background.  But to kickstart things, we'll
describe what to do.

The lab has five parts; 
  1. You control which of the five parts is being run by modifying
     the `part` variable in `fake-kernel/fake-os.c` and set it to `0`,
     `1`, etc.  You can also use the given enumeration type.

  2. You should do all `make` invocations in `fake-user-level` since
     that is what actually runs thigns.

What to do for each part:

  0. We give you the code for part 0.  This part just "loads" the
     user program (using `program_get`) and jumps to it directly, running
     it in kernel mode.  The user program does system calls to call back
     into the `fake-os`.  This gives you a simple starting point.

     *What to do*: follow the flow of control and make sure understand what
     is going on.  This is useful to help you get orientated with what
     the code is doing, since its a bit weird compared to the past labs.

     In the previous labs we just linked all the code together.  However,
     that doesn't make sense with user code.  ("Why?") Normally we would
     load a user program from a file system.  ("Why?")  However, we do
     not have a file system  (yet) and so do the following hack (which
     is useful elsewhere, so no worries).

     We use the program `fake-user-level/pitag-linker` to concatenate
     the user program to the `fake-os.bin`  and produce a new binary we
     can bootload over and jump to.  Mechanically this lets us combine
     two distinct programs.

     However, just as with linux and macos, the user cannot call the OS
     routines directly since it was not linked with them.  (Also, in the
     future: it does not have the right permissions).  So instead all
     "function calls" from user to the fake-os are system calls.  Because
     system call numbers do not change (ours are in `fake-os/syscalls.h`)
     the user code can call the `fake-os` no matter
     how its code has dilated or shrunk.

  1. You will write `user_mode_run_fn` (in `fake-os/user-mode-run-fn.S`).
     This will change to user mode (using the `cps` instruction),
     set the stack pointer to the value given, and jump to the code.
     This should be a short routine.

     Importantly: the code should run identically to part 0, other than
     perhaps printing out that it is at user-level.  You should rerun
     the programs using `part=0` and `part=1` and verify they both do
     the same thing.

  2. This part demonstrates how to use the single-stepping routines that we give
     you.  The exception handler used is in `fake-os/single-step.c`.  When you
     run with it enabled, it will print out all the `pc` addresses in order 
     that execute at user level (you cannot single-step when not in user mode).

     *What do to*: you should verify that the addresses being printed
     make sense.  They should first be after the `cps` instruction in
     part 1.  Then they will be in the `start` routine and `notmain` and
     then in the user-level code needed to call `sys_exit`.  Make sure you
     understand what is going on or the next parts will be hard to debug.

   3. You will trace all the program counter values and hash them.
      This code looks very similar to Part 2, except you need to figure
      out when the code has finished executing `user_mode_run_fn` (since
      its addresses will change).  The way you can do this is to look at
      the `pc` --- if this is above our kernel heap, it's in user mode.
      If not, its still in the kernel.

   4. Finally you will hash all the values and the `spsr`.  I'll write
      more about this soon.
