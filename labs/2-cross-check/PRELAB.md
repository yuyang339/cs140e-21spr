We're writing out the prelab.  For the momemt, you should look at:
  
   0. Setting your `CS140E_2021_PATH` variable (see below).

   1. `fake-pi` : trivial user-level emulator for your pi code.
      You will use this approach to cross-check your implementation
      against everyone else's to ensure everyone is equivalant.  Thus,
      if one person is correct, everyone is correct.

   2. `trace` : simple example of how to override routines using
      the linker.  You will use this to trace the PUT/GET calls when run
      on the actual pi hardware and do the same comparison to everyone
      else, and compare to when you replace our staff `gpio.o` with
      your own.

If you're going to modify these, please copy the directory --- I'm
still modifying and playing around with the code, so you'd get conflicts
otherwise.

To pop up a few levels, the raw tracing of PUT/GET enables some
interesting tricks:

  1. Check fidelity of your fake-pi emulator:  You can store the PUT/GET
     calls emitted from a raw run to a file (a log) and then change the
     fake pi so it optionally reads from this log: when you replay them
     the result should be identical.  (Including when you log the PUT/GET
     done during the replay, emit that to a log, replay that, etc.)

  2. Partial evaluation: You can also write a program that reads this
     log and emits a pi program that executes just these exact calls ---
     you can use this trick to replace the original program them with this
     (often much simpler) low-level sequence of PUTs and GETs.

-----------------------------------------------------------------
#### Part 0: Setup your  CS140E_2021_PATH variable.

Similar to how you added your local `~/bin` directory to your shell's path
on the first lab, today you're going to edit your shell configuration file
(e.g., `.tcshrc`, `.bash_profile`, etc) to set set an environment variable
`CS140E_PATH` to contain where you have your repository.

E.g., for me, since I'm running `tcsh` I edit my `.tcshrc` file and add:

    setenv CS140E_2021_PATH /home/engler/class/cs140e-21win-dev/

To the end of it.  If you use `bash` you'll do an:

    export CS140E_2021_PATH=<path to your repo>

And, as before, don't forget to source your configuration file.

As a simple test:

        % cd 2-cross-check/0-hello
        % make
        % pi-install hello.bin

Should print:

        ... bunch of stuff ...
        hello: things worked!
        DONE!!!

