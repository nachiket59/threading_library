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

int fun1(void* params){
	//sleep(10);
	// fprintf(stdout,"in the thread1\n");
	// fflush(stdout);
	while(1);
	int * ret = malloc(sizeof(int));
	*ret = 10;
	thread_exit((void*)ret);
}
int fun2(void* params){
		
	//sleep(15);
	// fprintf(stdout,"in the thread2\n");
	// fflush(stdout);
	int * ret = malloc(sizeof(int));
	*ret = 20;
	thread_exit((void*)ret);
}
int fun3(void* params){
	
	//sleep(20);
	// fprintf(stdout,"in the thread3\n");
	// fflush(stdout);
	int * ret = malloc(sizeof(int));
	*ret = 30;
	thread_exit((void*)ret);
}

void handle_int(){
	printf("killed\n");
	exit(0);
}

int main(){
	signal(SIGALRM, handle_int);
	int tid1, tid2, tid3;
	void * r2, *r1, *r3;
	thread_create(&tid1, NULL, fun1, NULL);
	thread_create(&tid2, NULL, fun2, NULL);
	thread_create(&tid3, NULL, fun3, NULL);
	thread_kill(tid1,SIGALRM);
	thread_join(tid1, &r1);
	thread_join(tid2, &r2);
	thread_join(tid3, &r3);
	printf("%d\n",*((int*)r1) );
	printf("%d\n",*((int*)r2) );
	printf("%d\n",*((int*)r3) );
	return 0;
}