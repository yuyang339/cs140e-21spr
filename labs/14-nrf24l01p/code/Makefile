# makefile configuration
# define this if you need to give the device for your pi
TTYUSB = 
# can override the default bootloader
BOOTLOADER = my-install

# the string to extract for checking
GREP_STR := 'HASH:\|ERROR:\|PANIC:\|PASS:\|TEST:'
CS140E_2021_PATH=/Users/yangyus/Documents/github/cs140e-21spr
# set if you want the code to automatically run after building.
RUN = 1
# set if you want the code to automatically check after building.
#CHECK = 1

TEST_SRC := $(wildcard ./[0-7]-test*.c)
TEST_SRC := $(wildcard ./tests/[12]-*.c)
TEST_SRC := 0-test-simple.c

# a list of all of your object files.
SRC :=  nrf-util.c
SUPPORT_OBJS := $(SRC:.c=.o)

# CFLAGS_EXTRA  = -Iexternal-code

# these are the binaries we give you: remove these when finished.
STAFF_OBJS =  spi.o

include $(CS140E_2021_PATH)/libpi/mk/Makefile.template
