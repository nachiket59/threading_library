/* A code demonstrating the race problem
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "../np_threadlib.h"

spin_lock s;
long c = 0, c1 = 0, c2 = 0, run = 1;
void* thread1(void *arg) {
	while(run == 1) {
		spin_lock_aquire(&s);
		c++;
		c1++;
		spin_lock_release(&s);
	}
}
void* thread2(void *arg) {
	while(run == 1) {
		spin_lock_aquire(&s);
		c++;
		c2++;
		spin_lock_release(&s);
	}
}
void handle_int(){
	run = 0;
}
int main() {
	thread_library_init();
	signal(SIGINT, handle_int);
	printf("Test case: Testing Spin lock\n");
	printf("In test2_2 Spin lock is used.\n");
	printf("use CTRL+C to stop execution and check consistency\n");
	int th1, th2; 
	thread_create(&th1, NULL, thread1, NULL);
	thread_create(&th2, NULL, thread2, NULL);

	while(run);
	
	thread_join(th1,NULL);
	thread_join(th2,NULL);
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
