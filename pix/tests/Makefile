MAKEFLAGS += --no-print-directory

# makefile configuration
# define this if you need to give the device for your pi
TTYUSB = 
# can override the default bootloader
BOOTLOADER = my-install

# the string to extract for checking
GREP_STR := 'EQUIV:\|TRACE:\|ERROR:\|PANIC:'

# set if you want the code to automatically run after building.
#RUN = 1
# set if you want the code to automatically check after building.
#CHECK = 1

# wait: this should work too, right?
# TEST_SRC := driver.c
# TEST_SRC := 2-test-fault.c
TEST_SRC := 0-test-exit.c
TEST_SRC := 3-test-vec.c
TEST_SRC := 0-test-exit.c 0-test-nop.c 3-test-vec.c
TEST_SRC := 1-test-hello.c
TEST_SRC := 0-test-nop.c
TEST_SRC := $(wildcard ./[0-9]-test*.c)

# put all your miscellaneous assembly in this file. 
SUPPORT_OBJS = 

# these are the .o's we give you: remove these when finished.
STAFF_OBJS =  

include $(CS140E_2021_PATH)/pix/mk/Makefile.template
