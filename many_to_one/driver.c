#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "np_threadlib.h"

void fun1(){
	while(1){
		sleep(1);
		printf("in the thread1\n");}
}
void fun2(){	
	while(1){
		sleep(1);
		printf("in the thread2\n");
	}	
}
void fun3(){
	while(1){
		sleep(1);
		printf("in the thread3\n");	
	}	
}

int main(){
	thread_library_init();

	int tid1, tid2, tid3;

	thread_create(&tid1, NULL, fun1, NULL);
	thread_create(&tid2, NULL, fun2, NULL);
	thread_create(&tid3, NULL, fun3, NULL);

	while(1);
	return 0;
}