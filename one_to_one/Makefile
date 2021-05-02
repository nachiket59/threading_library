CC=gcc
CFLAGS=-g -w

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h thread_list.c thread_list.h 
	$(CC) $(CFLAGS) -c np_threadlib.c
	$(CC) $(CFLAGS) -c  thread_list.c
	ld -relocatable np_threadlib.o thread_list.o -o np_threadlib	

clean:
	rm *.o
	rm np_threadlib