# makefile configuration
# define this if you need to give the device for your pi
TTYUSB = 
# can override the default bootloader
BOOTLOADER = my-install

# the string to extract for checking
GREP_STR := 'HASH:\|ERROR:\|PANIC:'

# set if you want the code to automatically run after building.
RUN = 1
# set if you want the code to automatically check after building.
CHECK = 1

TEST_SRC := $(wildcard ./[0-7]-test*.c)
TEST_SRC := driver.c

# put all your miscellaneous assembly in this file. 
SUPPORT_OBJS = mmu-helpers.o mmu.o  mmu-asm.o

# these are the binaries we give you: remove these when finished.
STAFF_OBJS =  staff-mmu-asm.o staff-mmu.o 

include $(CS140E_2021_PATH)/libpi/mk/Makefile.template
