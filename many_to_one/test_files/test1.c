#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "../np_threadlib.h"
int a,b,c;
void* fun1(void* params){
	a = 10;
	return 0;	
}
void* fun2(void* params){
	b =	20;
	return 0;
	
}
void* fun3(void* params){
	c = 30;
	return 0;
}


int main(){
	a = 0; b = 0; c = 0;
	printf("Test Case: Create three threads and Join three threads. Ensure all three execute properly.\n");
	int tid1, tid2, tid3;
	thread_library_init();
	thread_create(&tid1, NULL, fun1, NULL);
	thread_create(&tid2, NULL, fun2, NULL);
	thread_create(&tid3, NULL, fun3, NULL);
	thread_join(tid1, NULL);
	thread_join(tid2, NULL);
	thread_join(tid3, NULL);
	if(a == 10 && b == 20 && c==30){
		printf("PASSED\n");
	}
	else{
		printf("FAILED\n");
	}
	for(int i = 0; i<100; i++){
		printf("-");
	}
	printf("\n");
	return 0;
}