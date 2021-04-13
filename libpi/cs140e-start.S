#include "rpi-asm.h"

// To keep this in the first portion of the binary.
.section ".text.boot"

.globl _start
_start:
    mov sp, #STACK_ADDR
    mov fp, #0  // I don't think necessary.
    @ bl notmain
    bl _cstart
    bl rpi_reboot // if they return just reboot.

@ keep these here so that they are in the same location in every binary.
@ if you want to override them easily, can re-define the name in rpi-asm.h
.globl put32
.globl PUT32
put32:
PUT32:
    str r1,[r0]
    bx lr

.globl get32
.globl GET32

get32:
GET32:
    ldr r0,[r0]
    bx lr
