
@ hack for testing dumb swi calls.
#include "rpi-asm.h"

.globl _interrupt_table
.globl _interrupt_table_end
_interrupt_table:
  ldr pc, _reset_asm                    @ 0
  ldr pc, _undefined_instruction_asm    @ 1
  ldr pc, _software_interrupt_asm       @ 2
  ldr pc, _prefetch_abort_asm   @ 3
  ldr pc, _data_abort_asm   @4
  ldr pc, _reset_asm   @5
  ldr pc, _interrupt_asm @6
fast_interrupt_asm:
  mov sp, #INT_STACK_ADDR 
  sub   lr, lr, #4
  bl    fast_interrupt_vector

_reset_asm:                   .word bad_exception_asm
_undefined_instruction_asm:   .word bad_exception_asm
_software_interrupt_asm:      .word swi_trivial_vector
_prefetch_abort_asm:          .word bad_exception_asm
_data_abort_asm:              .word bad_exception_asm
_interrupt_asm:               .word bad_exception_asm
_interrupt_table_end:

MK_FN(software_interrupt_asm)
    mov sp, #INT_STACK_ADDR
    mov r0, lr
    b software_interrupt_vector


MK_FN(bad_exception_asm)
    mov sp, #INT_STACK_ADDR
    mov r0, lr
    b undefined_instruction_vector



@ just return: note we will trash super's lr
MK_FN(swi_trivial_vector)
    add r0, r0, #1
    movs pc, lr

@ save lr b/c hardware will trash it (since we are at SUPER)
MK_FN(swi_add)
    push {lr}
    swi 1
    pop {lr}
    bx lr

@
@ for use with 3-122 setting base vector.
@
@ One issue is that you will have to flush the data / instruction
@ cache when you modify this.
@ may want to go to a slower one if you want to override: load
@ from a table.
@
.align 5
.globl _interrupt_vector_slow
_interrupt_vector_slow:
  ldr pc, =bad_exception_asm
  ldr pc, =bad_exception_asm
  ldr pc, =swi_trivial_vector
  ldr pc, =bad_exception_asm
  ldr pc, =bad_exception_asm
  ldr pc, =bad_exception_asm
  ldr pc, =bad_exception_asm



.align 5
.globl _interrupt_vector_fast
_interrupt_vector_fast:
  ldr pc, _reset_asm
  ldr pc, _undefined_instruction_asm
  @ very cool.
  b swi_trivial_vector
  ldr pc, _prefetch_abort_asm
  ldr pc, _data_abort_asm
  ldr pc, _reset_asm
  ldr pc, _interrupt_asm


MK_FN(push_ret)
    push {lr}
    pop {lr}
    bx lr
