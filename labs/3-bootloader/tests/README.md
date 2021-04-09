Copy:

   1. Your tracing `my-install` as `~bin/my-install.trace`
   2. Your not-tracing `my-install` as `~bin/my-install`

If trace_p in my-install = 1 (these will run slow):  

        % cd output-boot-trace  
        % make check

If trace_p in my-install = 0:

        % cd output-no-boot-trace
        % make check
