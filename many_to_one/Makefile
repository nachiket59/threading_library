CC=gcc
CFLAGS=-g

run: driver.o np_threadlib.o thread_list.o thread_queue.o 
	$(CC) $(CFLAGS) -o driver driver.o np_threadlib.o thread_list.o thread_queue.o

driver: driver.c np_threadlib.h
	$(CC) $(CFLAGS) -c driver.c 

# run: race.o np_threadlib.o thread_list.o thread_queue.o
# 	$(CC) $(CFLAGS) -o race race.o np_threadlib.o thread_list.o thread_queue.o

# race: race.c np_threadlib.h
# 	$(CC) $(CFLAGS) -c race.c 

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h thread_queue.h 
	$(CC) $(CFLAGS) -c np_threadlib.c 

thread_list: thread_list.c thread_list.h 
	$(CC) $(CFLAGS) -c  thread_list.c

thread_queue: thread_queue.c thread_queue.h
	$(CC) $(CFLAGS) -c  thread_queue.c

clean:
	rm *.o