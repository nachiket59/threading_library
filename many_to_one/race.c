/* A code demonstrating the race problem
*/
#include <stdio.h>
#include <unistd.h>
#include "np_threadlib.h"

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
	int t1;
	thread_attributes ta;
	//init_thread_attributes(&ta);
	//ta.stack_size = 4096;
	//thread_create(&t1, NULL, thread1, NULL);
	while(run == 1) {
		spin_lock_aquire(&s);
		c++;
		c2++;
		spin_lock_release(&s);
	}
	
	//	thread_join(t1);
}
int main() {
	//spin_lock_init(&s);
	thread_library_init();
	int th1, th2; 
	thread_create(&th1, NULL, thread1, NULL);
	thread_create(&th2, NULL, thread2, NULL);
	//fprintf(stdout, "Ending main\n");
	sleep(2);
	run = 0;
	thread_join(th1);
	thread_join(th2);
	fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
	fflush(stdout);
	//spin_lock_destroy(s);
}
