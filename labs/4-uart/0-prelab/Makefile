# simple test that you have your path defined.
NAME = hello
CS140E_2021_PATH=/Users/yangyus/Documents/github/cs140e-21spr
# check that your path is defined.
ifndef CS140E_2021_PATH
$(error CS140E_2021_PATH is not set: this should contain the absolute path to where this directory is.  Define it in your shell's initialiation.  For example, .tcshrc for tcsh or .bashrc for bash)
endif

include $(CS140E_2021_PATH)/libpi/includes.mk
CFLAGS += -Wno-unused-function -Wno-unused-variable -falign-functions=4 

# add all the .h's as dependencies, libpi and the makefile.
DEPS := $(MEMMAP) $(LPI) $(wildcard ./*.h) ./Makefile 

LIBS = $(LPI)  

all: libs $(OBJS) $(NAME).bin # run

libs: 
	make -C $(LPP)

$(OBJS): $(DEPS)
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) -c $(CPP_ASFLAGS) $< -o $@

$(NAME).elf: $(NAME).o $(MEMMAP) $(DEPS) 
	$(LD) $(START) $< -T $(LPP)/memmap -o $@  $(LIBS)

%.list: %.elf
	$(OD) -D   $*.elf > $*.list
%.bin: %.list
	$(OCP) $*.elf -O binary $*.bin

# your my-install should figure this out
run: $(NAME).bin
	my-install $(NAME).bin

tags: 
	ctags *.[chSs] $(LPP)/*.[ch] $(LPP)/*.[chSs]

clean:
	rm -rf $(NAME) *~ tags *.o *.bin *.elf *.list *.img Makefile.bak  *.d

.PHONY: libs all tags clean run

# stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf
