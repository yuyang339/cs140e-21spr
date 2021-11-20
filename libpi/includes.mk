# setup useful variables that can be used by make.

ARM = arm-none-eabi
# CS140E_2021_PATH=/Users/yangyus/Documents/github/cs140e-21spr

CC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
AR = $(ARM)-ar
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy
CS140E_2021_LIBPI_PATH = $(CS140E_2021_PATH)/libpi
LPP = $(CS140E_2021_LIBPI_PATH)
LPI = $(CS140E_2021_LIBPI_PATH)/libpi.a
START = $(LPP)/cs140e-start.o

MEMMAP=$(LPP)/memmap

ifndef CS140E_2021_PATH
$(error CS140E_2021_PATH is not set: this should contain the absolute path to where this directory is.  Define it in your shell's initialiation.  For example, .tcshrc for tcsh or .bashrc for bash)
endif

ifndef INC
INC = -I$(LPP)/include -I$(LPP)/ -I$(LPP)/src -I.
endif

ifndef OPT_LEVEL
OPT_LEVEL = -Og
endif

CFLAGS = $(OPT_LEVEL) -Wall -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s  -std=gnu99 $(INC) -ggdb -Wno-pointer-sign  $(CFLAGS_EXTRA)

ASFLAGS = --warn --fatal-warnings  -mcpu=arm1176jzf-s -march=armv6zk $(INC)

CPP_ASFLAGS =  -nostdlib -nostartfiles -ffreestanding   -Wa,--warn -Wa,--fatal-warnings -Wa,-mcpu=arm1176jzf-s -Wa,-march=armv6zk   $(INC)
