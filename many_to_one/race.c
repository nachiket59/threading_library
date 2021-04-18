/* A code demonstrating the race problem
*/
#include <stdio.h>
#include <unistd.h>
#include "np_threadlib.h"
#include <signal.h>
#include <stdlib.h>
spin_lock s;
mutex_lock m_lock;
long c = 0, c1 = 0, c2 = 0, run = 1;
void* thread1(void *arg) {
	while(run == 1) {
		// spin_lock_aquire(&s);
		thread_mutex_lock(&m_lock);
		c++;
		c1++;
		thread_mutex_unlock(&m_lock);
		// spin_lock_release(&s);
	}
}
void* thread2(void *arg) {
	int t1;
	thread_attributes ta;
	init_thread_attributes(&ta);
	ta.stack_size = 4096;
	thread_create(&t1, NULL, thread1, NULL);
	while(run == 1) {
		// spin_lock_aquire(&s);
		thread_mutex_lock(&m_lock);
		c++;
		c2++;
		thread_mutex_unlock(&m_lock);
		// spin_lock_release(&s);
	}
	thread_join(t1);
}
void int_handler(){
	fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
	fflush(stdout);
	exit(0);
}
int main() {
	signal(SIGINT, int_handler);
	thread_mutex_init(&m_lock);
	//spin_lock_init(&s);
	thread_library_init();
	int th1, th2; 
	thread_create(&th1, NULL, thread1, NULL);
	thread_create(&th2, NULL, thread2, NULL);
	//fprintf(stdout, "Ending main\n");
	thread_join(th1);
	thread_join(th2);
	//spin_lock_destroy(s);
}
