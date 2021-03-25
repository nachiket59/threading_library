#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* fun1(void* params){
	sleep(10);
	printf("in the thread1\n");
}
void* fun2(void* params){	
	sleep(15);
	printf("in the thread2\n");
}
void* fun3(void* params){
	sleep(20);
	printf("in the thread3\n");
}

int main(){
	pthread_t tid1,tid2,tid3;
	pthread_create(&tid1, NULL, fun1, NULL);
	pthread_create(&tid2, NULL, fun2, NULL);
	pthread_create(&tid3, NULL, fun3, NULL);
	pthread_join(tid1,NULL);
	printf("here\n");
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	printf("end of main\n");
	return 0;
}