#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "np_threadlib.h"

int fun(void* params){
	printf("in the thread\n");
	sleep(15);
}

int main(){
	int tid;
	thread_create(&tid, NULL, fun, NULL);
	thread_join(tid);
	printf("in the thread %d\n", tid);
	return 0;
}