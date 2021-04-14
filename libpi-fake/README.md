TODO:
   - copy your `gpio.c` and `uart.c` into `libpi/src`.

   - Put your fake memory prototypes in: `fake-mem.h`
   - Copy your `fake-put-get.c` here.
   - You'll have to add a `case` label in `GET32` for 

            uart_mu_stat_reg = 0x20215064,

     just like `gpio_lev0`.

   - `cd tests-gpio; make check` should pass
   - then start working on the tests in `tests-uart`.
