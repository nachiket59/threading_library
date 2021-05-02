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

runnable:np_threadlib test1.o test2_1.o test2_2.o test2_3.o test3.o test4_1.o test4_2.o test4_3.o matrix.o
	$(CC) $(CFLAGS) -o test1 test1.o np_threadlib 
	$(CC) $(CFLAGS) -o test2_1 test2_1.o np_threadlib
	$(CC) $(CFLAGS) -o test2_2 test2_2.o np_threadlib
	$(CC) $(CFLAGS) -o test2_3 test2_3.o np_threadlib
	$(CC) $(CFLAGS) -o test3 test3.o np_threadlib
	$(CC) $(CFLAGS) -o test4_1 test4_1.o np_threadlib
	$(CC) $(CFLAGS) -o test4_2 test4_2.o np_threadlib
	$(CC) $(CFLAGS) -o test4_3 test4_3.o np_threadlib
	$(CC) $(CFLAGS) -o matrix matrix.o np_threadlib


test_files:../np_threadlib.h test1.c test2_1.c test2_2.c test2_3.c test3.c test4_1.c test4_2.c test4_3.c matrix.c
	$(CC) $(CFLAGS) -c test1.c test2_1.c test2_2.c test2_3.c test3.c test4_1.c test4_2.c test4_3.c matrix.c
	

np_threadlib: ../np_threadlib.c ../np_threadlib.h ../thread_list.h ../thread_list.c ../thread_list.h ../thread_queue.c ../thread_queue.h
	$(CC) $(CFLAGS) -c ../np_threadlib.c
	$(CC) $(CFLAGS) -c  ../thread_list.c
	$(CC) $(CFLAGS) -c  ../thread_queue.c
	ld -relocatable np_threadlib.o thread_list.o thread_queue.o -o np_threadlib	

clean:
	rm *.o
	rm np_threadlib

run:
	bash runtests.bash