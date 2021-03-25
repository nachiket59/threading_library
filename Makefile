CC=gcc
CFLAGS=-g

run: driver.o np_threadlib.o thread_list.o
	$(CC) $(CFLAGS) -o driver driver.o np_threadlib.o thread_list.o

driver: driver.c np_threadlib.h
	$(CC) $(CFLAGS) -c driver.c 

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h
	$(CC) $(CFLAGS) -c np_threadlib.c 

threading_list: thread_list.c thread_list.h 
	$(CC) $(CFLAGS) -c  thread_list.c
