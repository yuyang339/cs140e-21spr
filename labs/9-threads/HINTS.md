Various hints:
  - we probably should have called `saved_sp` just `sp` since it is always
    the value of the associated thread's  `sp` register. 

  - make sure you initialize `sp_saved` in `rpi_fork`.

  - the only place that the `stack` field should be referenced is at
    the start of `rpi_fork`.  You should only use `saved_sp` everywhere
    else.

  - if you're having trouble with trampoline: one helpful fact is that 
    `rpi_cswitch` always jumps to whatever is in the `lr` register.
    It loads this from the stack.  So if we can write that offset (which
    should at `LR_OFFSET` as per the `2-write-regs.c` test case) then
    we can make `rpi_cswitch` jump to the trampoline.
