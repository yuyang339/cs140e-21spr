# makefile configuration
# define this if you need to give the device for your pi
TTYUSB = 
# can override the default bootloader
BOOTLOADER = my-install

# the string to extract for checking
GREP_STR := 'HASH:\|ERROR:\|PANIC:\|PASS:\|TEST:'

# set if you want the code to automatically run after building.
RUN = 0
# set if you want the code to automatically check after building.
#CHECK = 0

TEST_SRC := $(wildcard ./[0-7]-test*.c)
TEST_SRC := $(wildcard ./tests/[12]-*.c)
TEST_SRC := 0-test-simple.c
TEST_SRC := 1-client-one-way.c  1-server-one-way.c
TEST_SRC := 2-client-pingpong.c  2-server-pingpong.c
TEST_SRC := 1-client-one-way.c  1-server-one-way.c 2-client-pingpong.c  2-server-pingpong.c

# a list of all of your object files.
SRC :=  nrf-util.c nrf-driver.c nrf-public.c 
# staff-nrf-driver.c
SUPPORT_OBJS := $(SRC:.c=.o) 

# CFLAGS_EXTRA  = -Iexternal-code

# these are the binaries we give you: remove these when finished.
STAFF_OBJS = staff-nrf-driver.o

include $(CS140E_2021_PATH)/libpi/mk/Makefile.template


crun1: 1-client-one-way.bin
	$(BOOTLOADER) /dev/ttyUSB0 $<

# run server: start this second.
srun1: 1-server-one-way.bin
	$(BOOTLOADER) /dev/ttyUSB1 $<

# run client: start this first
crun: 2-client-pingpong.bin
	$(BOOTLOADER) /dev/ttyUSB0 2-client-pingpong.bin

# run server: start this second.
srun: 2-server-pingpong.bin
	$(BOOTLOADER) /dev/ttyUSB1 2-server-pingpong.bin
