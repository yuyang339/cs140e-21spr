#include "rpi-asm.h"

#define not_reached()  mov r0, pc; b asm_not_reached
#define unimplemented()  mov r0, pc; b asm_unimplemented


@ does not return.
MK_FN(user_mode_run_fn)
    @ use cps to switch to user level.
    @ make sure to do a "prefetch flush" to 
    @ flush the state  (see rpi-asm.h)
    @
    @ for later, you should clear all the registers.
    unimplemented()
