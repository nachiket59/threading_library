/* A code demonstrating the race problem
*/
#include <stdio.h>
#include <unistd.h>
#include "np_threadlib.h"

mutex_lock m_lock;
long c = 0, c1 = 0, c2 = 0, run = 1;
int thread1(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&m_lock);
		c++;
		c1++;
		thread_mutex_unlock(&m_lock);
	}
}
int thread2(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&m_lock);
		c++;
		c2++;
		thread_mutex_unlock(&m_lock);
	}
}

int main() {
	printf("Test case: Testing mutex lock\n");
	printf("In test2_2 mutex lock is used.\n");
	thread_mutex_init(&m_lock);
	int th1, th2; 
	thread_create(&th1, NULL, thread1, NULL);
	thread_create(&th2, NULL, thread2, NULL);
	sleep(2);
	run = 0;
	thread_join(th1, NULL);
	thread_join(th2, NULL);
	fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
	fflush(stdout);
	if(c == c1+c2){
		printf("consistant output\n");
		printf("PASSED\n");
	}
	else{
		printf("FAILED\n");
	}
	for(int i = 0; i<100; i++){
		printf("-");
	}
	printf("\n");
}
