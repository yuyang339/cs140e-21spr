# if you want to replace our foo.o with yours do as follows.
#   0. copy foo.c to libpi/src
#   1. add src/foo.c to YOUR_SRC
#   2. remove foo.o from STAFF_OBJS

# put the path to any .S files.
YOUR_ASM = 

# put the path to any .c files
YOUR_SRC = src/hello.c src/gpio.c src/uart.c

# these are initial .o's we give you: to use your own, remove them
STAFF_OBJS= staff-objs/gpio-pud.o 
