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

int t[100];
int a;
spin_lock s;
void* fun1(void* params){
	spin_lock_aquire(&s);
	a=a+1;
	spin_lock_release(&s);
}


int main(){
	printf("Test Case: Creating 100 threads.\n");
	
	a = 0;
	int i;
	
	for(i = 0; i< 100; i++){
		thread_create(&t[i], NULL, fun1, NULL);
	}

	for(i = 0; i< 100; i++){
		thread_join(t[i], NULL);
	}

	if(a==100){
		printf("PASSED\n");
	}
	else{
		printf("FAILED\n");
	}
	for(i = 0; i<100; i++){
		printf("-");
	}
	printf("\n");
	return 0;
}