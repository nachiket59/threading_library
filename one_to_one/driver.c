#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "np_threadlib.h"

int fun1(void* params){

	sleep(10);
	printf("in the thread1\n");
}
int fun2(void* params){	
	sleep(15);
	printf("in the thread2\n");
}
int fun3(void* params){
	sleep(20);

	printf("in the thread3\n");
}

int main(){


	int tid1, tid2, tid3;

	thread_create(&tid1, NULL, fun1, NULL);
	thread_create(&tid2, NULL, fun2, NULL);
	thread_create(&tid3, NULL, fun3, NULL);
	thread_join(tid1);

	thread_join(tid2);
	thread_join(tid3);

	return 0;
}