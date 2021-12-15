NAME = pitag-linker

CC = gcc

LIBUNIX_PATH := $(CS140E_2021_PATH)/libunix
LIBUNIX := $(LIBUNIX_PATH)/libunix.a
CFLAGS := -Wall -Werror -g -Wno-unused-function -Wno-unused-variable -I. -I$(LIBUNIX_PATH) -g -DRPI_UNIX

# just make all .o's depend on everything.  simple.
SOURCES := $(wildcard ./*.c)
HDRS := $(wildcard ./*.h)
ALL_OBJS := $(patsubst %.c, %.o, $(SOURCES)) 

DEPS := ./Makefile $(LIBUNIX) $(LIBUNIX_PATH)/libunix.h $(HDRS)

all : libs $(NAME) 

# make libunix quietly.  remove the '@' and the '-s' if you want to see what is 
# going on.
libs:
	@make -s -C $(LIBUNIX_PATH)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@


$(NAME): $(ALL_OBJS) $(DEPS)
	$(CC) $(CFLAGS) $< -o $(NAME) $(LIBUNIX)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleanall)
-include $(dep)  # include dep files
endif
endif

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$

clean :
	rm -f *.o *~ Makefile.bak $(NAME)



.PHONY: clean tags libs run
