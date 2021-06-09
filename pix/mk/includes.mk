PIX_PATH = $(CS140E_2021_PATH)/pix

PIX_KERNEL_PATH = $(PIX_PATH)/pix-kernel$(STAFF)
PIX_USER_PATH = $(PIX_PATH)/pix-user
PITAG_PATH = $(PIX_PATH)/pitag-linker

PIX_BIN = $(PIX_KERNEL_PATH)/pix-kernel.bin
PITAG_LINKER = $(PITAG_PATH)/pitag-linker
PIX_USER = $(PIX_USER_PATH)/libpix.a

# ugh: includes defines it
INC = -I$(PIX_USER_PATH) -I$(PIX_KERNEL_PATH) -I$(CS140E_2021_PATH)/libpi/include -I.
include $(CS140E_2021_PATH)/libpi/includes.mk

CFLAGS += -Wno-unused-function -Wno-unused-variable  -I$(PIX_KERNEL_PATH) -I.
START := $(PIX_USER_PATH)/start.o
MEMMAP := $(PIX_USER_PATH)/memmap
