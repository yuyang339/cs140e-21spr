NAME = pix-kernel

# define this if you need to give the device for your pi
TTYUSB = 


# these are .o's only --- you should swap in your versions.
STAFF_OBJS = staff-breakpoint.o staff-mmu.o staff-mmu-asm.o staff-pix-asm.o staff-equiv.o

# these should be yours
YOUR_OBJS := # pix-asm.o equiv.o

OBJS := pix.o  $(YOUR_OBJS) \
        staff-pmap.o staff-vm.o staff-env.o  \
        staff-loader.o 
 
dep := $(OBJS:.o=.d) $(STAFF_OBJS:.o=.d)

ALL_STUDENT_FILES =             \
            Makefile            \
            breakpoint.h        \
            pitag.h             \
            pix.c               \
            pix-constants.h     \
            pix-internal.h      \
            reg-util.h          \
            staff-breakpoint.o  \
            staff-env.c         \
            staff-loader.c      \
            staff-pmap.c        \
            staff-vm.c          \
            syscalls.h          \
            vector-base.h       \
            staff-equiv.o       \
            staff-mmu-asm.o     \
            staff-mmu.o         \
            staff-pix-asm.o 


# VPATH := $(sort $(dir $(SRC)))
include $(CS140E_2021_PATH)/libpi/includes.mk
CFLAGS += -Wno-unused-function -Wno-unused-variable  -Werror

# add the memmap file, libpi and the makefile as dependencies
DEPS := $(MEMMAP) $(LPI) ./Makefile 

all: libs $(NAME).bin tags # run


libs: 
	make -C $(LPP)

$(OBJS): $(DEPS)
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) -c $(CPP_ASFLAGS) $< -o $@
%.elf: $(OBJS) $(DEPS) $(STAFF_OBJS)  
	$(LD) $(START)  $(OBJS) $(STAFF_OBJS) -T $(MEMMAP) -o $@  $(LPI)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleanall)
-include $(dep)  # include dep files
endif
endif

./%.d: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.list: %.elf
	$(OD) -D   $*.elf > $*.list
%.bin: %.list
	$(OCP) $*.elf -O binary $*.bin

run: $(NAME).bin
	make -C .. run
check: $(NAME).bin
	make -C .. check
# my-install $(TTYUSB) $(NAME).bin

update:
	cp $(ALL_STUDENT_FILES) ../pix-kernel

tags: 
	ctags *.[chSs] $(LPP)/*.[Sch] $(LPP)/*/*.[chS]

clean:
	rm -rf $(NAME) *~ tags $(OBJS) *.bin *.elf *.list *.img Makefile.bak  *.d 

.PHONY: libs all tags clean run check

# stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf %.o
