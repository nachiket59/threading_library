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

int tid1, tid2, tid3;
int a,b;
int fun1(void* params){
	thread_kill(tid1,SIGTERM);
	a= 10;
}
int fun2(void* params){
	b = 20;
}


int main(){
	printf("Test Case: Testing thread_kill function with SIGTERM signal.\n");
	a = 0; b = 0;
	thread_create(&tid1, NULL, fun1, NULL);
	thread_create(&tid2, NULL, fun2, NULL);
	thread_join(tid1, NULL);
	thread_join(tid2, NULL);
	if(a==0 && b == 20){
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