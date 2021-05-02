# Implementation of Multi-threading Library in C

This is an implementation of multi-threading library in C. There are two separate folders containing two different libraries implemented using two different thread scheduling approaches.
1. One to One approach. (one user thread is associated with one kernel thread).
2. Many to One Approach . (Many user threads are associated to one kernel thread).

## Steps to compile and use the library.
Run following commands.

    git clone https://gitlab.com/nachiket59/threading_library
There are two folders in the repository containing two different implementations. Navigate to whichever folder (implementation) and run following commands.

    make
Use the above command to compile the library. This will crate ***np_threadlib*** file, then include ***np_threadlib.h*** into your C code and compile.
For example you have a file code.c.

> #include "np_threadlib.h"

    gcc -c code.c
    gcc -o code code.o np_threadlib
    ./code

Each folder contains ***testing_files*** folder. Navigate to that folder and run following commands in order to perform conformance tests.

    make
    make run
This will start execution of multiple C codes written to perform various tests.
 


