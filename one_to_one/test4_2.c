#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "np_threadlib.h"

int tid1, tid2, tid3;
int a,b;
int fun1(void* params){
	a =10;
}

int main(){
	
	printf("Test Case: Testing thread_kill function with SIGTSTP and SIGCONT signal.\n");
	a = 0; b = 0;
	thread_create(&tid1, NULL, fun1, NULL);
	
	thread_kill(tid1,SIGTSTP);
	thread_kill(tid1, SIGCONT);

	thread_join(tid1, NULL);
	if(a==10 ){
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