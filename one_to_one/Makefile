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

runnable:np_threadlib test1.o test2_1.o test2_2.o test2_3.o test3.o test4_1.o test4_2.o test4_3.o
	$(CC) $(CFLAGS) -o test1 test1.o np_threadlib 
	$(CC) $(CFLAGS) -o test2_1 test2_1.o np_threadlib
	$(CC) $(CFLAGS) -o test2_2 test2_2.o np_threadlib
	$(CC) $(CFLAGS) -o test2_3 test2_3.o np_threadlib
	$(CC) $(CFLAGS) -o test3 test3.o np_threadlib
	$(CC) $(CFLAGS) -o test4_1 test4_1.o np_threadlib
	$(CC) $(CFLAGS) -o test4_2 test4_2.o np_threadlib
	$(CC) $(CFLAGS) -o test4_3 test4_3.o np_threadlib


test_files:np_threadlib.h test1.c test2_1.c test2_2.c test2_3.c test3.c test4_1.c test4_2.c test4_3.c
	$(CC) $(CFLAGS) -c test1.c test2_1.c test2_2.c test2_3.c test3.c test4_1.c test4_2.c test4_3.c

lib: np_threadlib.o thread_list.o
	ld -relocatable np_threadlib.o thread_list.o -o np_threadlib

np_threadlib: np_threadlib.c np_threadlib.h thread_list.h
	$(CC) $(CFLAGS) -c np_threadlib.c 

threading_list: thread_list.c thread_list.h 
	$(CC) $(CFLAGS) -c  thread_list.c
clean:
	rm *.o

run:
# 	./test1 
# 	./test2_1
# 	./test2_2
# 	./test2_3
# 	./test3
# 	./test4_1
# 	./test4_2
	./test4_3