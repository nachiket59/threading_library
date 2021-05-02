CC=gcc
CFLAGS=-g -w

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h thread_queue.h 
	$(CC) $(CFLAGS) -c np_threadlib.c 

thread_list: thread_list.c thread_list.h 
	$(CC) $(CFLAGS) -c  thread_list.c

thread_queue: thread_queue.c thread_queue.h
	$(CC) $(CFLAGS) -c  thread_queue.c

clean:
	rm *.o