### Unix Side

If you see errors about missing symbols when linking, make sure you copied the 
files you wrote in the prelab into `libunix`.

If you see errors that claim that a function isn't defined, check your 
includes.  Macs sometimes require include statements in header files where 
Linux doesn't, so you may have to add the right one to your header file.

If you get a segfault/bus error on a Mac, debug using `lldb my-install -- 
hello.bin`.  At the prompt, type `run`, then `bt` to print a backtrace.  This 
should identify the location of the error.  Linux users can use `gdb` to do the 
same.

Check the prototype of all the str/mem functions you use!  Some of them take a 
buffer as the first argument, some don't.  `strcatf` has tripped up a few 
people.

Don't worry about memory usage; use `strdup` to copy strings if necessary (e.g. 
to copy from the stack to the heap).
