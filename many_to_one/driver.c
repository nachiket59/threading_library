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
int a;
void* fun1(void* args){
	//thread_kill(tid3, SIGTERM);
	// while(1){
	// 	sleep(1);
	int* p = malloc(sizeof(int));
	*p = 10;
	thread_exit((void*)p);
	a=10;
		printf("in the thread1\n");
	}
// }
void* fun2(void* args){	
	// while(1){
	// 	sleep(1);
	int* p = malloc(sizeof(int));
	*p = 20;
	thread_exit((void*)p);
	a = 20;
		printf("in the thread2\n");
	}	
// }
void* fun3(void* args){
	// while(1){
	// 	sleep(1);
	int* p = malloc(sizeof(int));
	*p = 30;
	thread_exit((void*)p);
	a =30;
		printf("in the thread3\n");	
	}	
// }

int main(){
	a = 0;
	void *p1, *p2, *p3;
	thread_library_init();

	thread_create(&tid1, NULL, fun1, NULL);
	thread_create(&tid2, NULL, fun2, NULL);
	thread_create(&tid3, NULL, fun3, NULL);
	printf("created\n");
	thread_join(tid1, &p1);
	thread_join(tid2, &p2);
	thread_join(tid3, &p3);
	printf("%d\n",*((int*)p3));
	printf("%d\n",a);
	sleep(5);
	//while(1)printf("no\n");;
	return 0;
}