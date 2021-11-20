MAKEFLAGS += --no-print-directory
CS140E_2021_PATH=/Users/yangyus/Documents/github/cs140e-21spr
PIX_PATH = $(CS140E_2021_PATH)/pix
include $(PIX_PATH)/mk/includes.mk

# your .o's
SRC = $(wildcard $(LPP)/libc/mem*.c $(LPP)/libc/mem*.c)
OBJS := $(patsubst %.c, %.o, $(SRC))
OBJS := $(foreach obj, $(OBJS), ./$(notdir $(obj)))

SUPPORT_OBJS = ./pix-user-level.o  ./invoke-syscall.o 
SUPPORT_OBJS += $(OBJS)

STAFF_OBJS =
TARGET = libpix.a

dep := $(SUPPORT_OBJS:.o=.d)

VPATH := $(sort $(dir $(SRC)))

# add the memmap file, libpi and the makefile as dependencies
DEPS = $(MEMMAP) $(LPI) ./Makefile 

ALL_OBJS := $(SUPPORT_OBJS) $(STAFF_OBJS) 

all: libs $(TARGET)

libs: 
	make -C $(LPP)
	make -C $(PIX_KERNEL_PATH)

$(SUPPORT_OBJS): $(DEPS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S $(DEPS)
	$(CC) -c $(CPP_ASFLAGS) $< -o $@

$(TARGET): $(ALL_OBJS) $(DEPS) $(START) $(MEMMAP)
	arm-none-eabi-ar crf $(TARGET) $(ALL_OBJS) 
	$(OD) -D $@ > $@.list

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleanall)
-include $(dep)  # include dep files
endif
endif

./%.d: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

tags: 
	ctags *.[chSs] $(LPP)/*.[ch] $(LPP)/*.[chSs]

clean:
	rm -rf $(TARGET) *~ tags $(SUPPORT_OBJS) *.bin *.elf *.list *.img Makefile.bak  *.d  $(TEST_OBJS) *.ptag start.o

.PHONY: libs all tags clean run update cleanall

# stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf %.o %.d
