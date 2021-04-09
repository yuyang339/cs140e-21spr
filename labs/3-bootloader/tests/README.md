Copy:

   1. Your tracing `my-install` as `~bin/my-install.trace`
   2. Your not-tracing `my-install` as `~bin/my-install`

Note:
  - Don't trace `put_put8`.

If trace_p in my-install = 1 (these will run slow):  

        % cd output-boot-trace  
        % make check

If trace_p in my-install = 0:

        % cd output-no-boot-trace
        % make check


----------------------------------------------------------------------
### For reference

No tracing (`my-install`):

    % my-install hello.bin
    my-install: tty-usb=</dev/ttyUSB0> program=<hello.bin> about to boot
    TRACE:simple_boot: sending 3372 bytes, crc32=cf4943ae
    waiting for a start
    putting code
    bootloader: Done.
    listening on ttyusb=</dev/ttyUSB0>
    hello world
    DONE!!!
    
    Saw done

With tracing (`my-install.trace`):

    % my-install.trace hello.bin 
    FOUND: </dev/ttyUSB0>
    opened tty port </dev/ttyUSB0>.
    my-install.trace: tty-usb=</dev/ttyUSB0> program=<hello.bin> about to boot
    TRACE:simple_boot: sending 3372 bytes, crc32=cf4943ae
    waiting for a start
    TRACE:GET32:11112222
    TRACE:PUT32:33334444
    TRACE:PUT32:8000
    TRACE:PUT32:d2c
    TRACE:PUT32:cf4943ae
    TRACE:GET32:55556666
    TRACE:GET32:cf4943ae
    putting code
    TRACE:PUT32:77778888
    TRACE:GET32:9999aaaa
    bootloader: Done.
    listening on ttyusb=</dev/ttyUSB0>
    hello world
    DONE!!!
    
    Saw done
    
