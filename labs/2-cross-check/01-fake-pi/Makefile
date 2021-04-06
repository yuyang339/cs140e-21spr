# define where gpio.c is, and the programs you want to run.
CODE_PATH = ../../1-gpio/code/
PROGS        := 1-blink 2-blink 3-input
GPIO         := $(CODE_PATH)/gpio.c

CODE         := fake-pi.c $(GPIO)

CC = gcc
CFLAGS = -I$(CODE_PATH) -Wall -Wno-unused-function -Wno-unused-variable 

all: $(PROGS)

$(PROGS):%: $(CODE_PATH)/%.c  $(CODE)
	$(CC) $(CFLAGS) $< $(CODE) -o $@ 

clean:
	rm -rf $(PROGS) *~ tags *.o Makefile.bak  

.PHONY: all clean run

