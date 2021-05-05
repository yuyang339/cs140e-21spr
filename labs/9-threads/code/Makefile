# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = my-install

TEST_SRC := 0-test-push.c # 4-test-exit.c # 1-test-thread.c 3-test-cswitch.c
TEST_SRC := $(wildcard ./[0]-test*.c)
TEST_SRC := $(wildcard ./[0]*.c)
TEST_SRC := 0-where-push.c 0-write-regs.c
TEST_SRC := $(wildcard ./[1]-test*.c)
TEST_SRC := $(wildcard ./[4]-test*.c)
TEST_SRC := $(wildcard ./[5]-test*.c)
TEST_SRC := $(wildcard ./[2]-*.c)
TEST_SRC := $(wildcard ./[1]-*.c)

# put all your miscellaneous assembly in this file. 
SUPPORT_OBJS := lab-c-helpers.o rpi-thread.o  thread-asm.o

include Makefile.template
