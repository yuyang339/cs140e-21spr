// we need to keep this in another file or gcc inlines / removes it. 
#include <stdio.h>
#include "foo.h"

unsigned foo(unsigned arg0, unsigned arg1) {
    printf("real foo: have arguments (%d,%d)\n", arg0,arg1);
    return arg0 + arg1;
}

unsigned ___real_foo(unsigned arg0, unsigned arg1) {
    printf("real foo: have arguments (%d,%d)\n", arg0,arg1);
    return arg0 + arg1;
}

unsigned ___wrap_foo(unsigned arg0, unsigned arg1) {
    printf("real foo: have arguments (%d,%d)\n", arg0,arg1);
    return arg0 + arg1;
}