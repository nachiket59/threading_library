CC=gcc
CFLAGS=-I.

run: driver.o np_threadlib.o thread_list.o
	$(CC) -o driver driver.o np_threadlib.o thread_list.o

driver: driver.c np_threadlib.h
	$(CC) -c driver.c 

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h
	$(CC) -c np_threadlib.c 

threading_list: thread_list.c thread_list.h
	$(CC) -c  thread_list.c
