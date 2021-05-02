CC=gcc
CFLAGS=-g

# run: driver.o np_threadlib.o thread_list.o
# 	$(CC) $(CFLAGS) -o driver driver.o np_threadlib.o thread_list.o

# driver: driver.c np_threadlib.h
# 	$(CC) $(CFLAGS) -c driver.c 

# run: race.o np_threadlib.o thread_list.o
# 	$(CC) $(CFLAGS) -o race race.o np_threadlib.o thread_list.o

# race: race.c np_threadlib.h
# 	$(CC) $(CFLAGS) -c race.c

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h thread_list.c thread_list.h 
	$(CC) $(CFLAGS) -c np_threadlib.c
	$(CC) $(CFLAGS) -c  thread_list.c
	ld -relocatable np_threadlib.o thread_list.o -o np_threadlib	

clean:
	rm *.o
	rm np_threadlib