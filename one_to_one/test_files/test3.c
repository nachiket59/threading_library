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

int fun1(void* params){
	int * ret = malloc(sizeof(int));
	*ret = *((int*)params);
	thread_exit((void*)ret);
}
int fun2(void* params){
	int * ret = malloc(sizeof(int));
	*ret = *((int*)params);
	thread_exit((void*)ret);
}
int fun3(void* params){
	int * ret = malloc(sizeof(int));
	*ret = *((int*)params);
	thread_exit((void*)ret);
}

int main(){
	printf("Test case: Testing parameter passing into thread and thread_exit() fuction\n");
	printf("Each thread returns the value passed as parameter using thread_exit() function\n");
	printf("Passing values 10, 20, 30 in 3 threads. Expected result - Thread returns the same value\nThis value can be accessed at the addreess given in second parameter of thread_join() function.\n");
	int tid1, tid2, tid3;
	void * r2, *r1, *r3;
	int p1 = 10, p2 =20, p3 =30;
	thread_create(&tid1, NULL, fun1, (void*)&p1);
	thread_create(&tid2, NULL, fun2, (void*)&p2);
	thread_create(&tid3, NULL, fun3, (void*)&p3);
	thread_join(tid1, &r1);
	thread_join(tid2, &r2);
	thread_join(tid3, &r3);
	printf("%d\n",*((int*)r1) );
	printf("%d\n",*((int*)r2) );
	printf("%d\n",*((int*)r3) );
	if(*((int*)r1) == 10 && *((int*)r2)==20 && *((int*)r3) == 30){
		printf("PASSED\n");
	}
	for(int i = 0; i<100; i++){
		printf("-");
	}
	printf("\n");
	return 0;
}