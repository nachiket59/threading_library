#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "np_threadlib.h"
#include "thread_list.h"

thread_list* tlist_start = NULL;

spin_lock thread_list_lock;
void init_thread_attributes(thread_attributes * attributes){
	attributes->detachable = DETACHABLE;
	attributes->scheduling_policy = ONE_TO_ONE;
	attributes->stack_size = DEFAULT_STACK_SIZE;
	return; 
}

int thread_create_one_one(int* tid, thread_attributes* attributes, int (*fun)(void*), void* parameters){
	int pid;
	thread_control_block tcb;
	char* stack = malloc(attributes->stack_size);
	  if (!stack) {
	    perror("malloc error");
	    return 1;
	}

	pid = clone(*fun, stack + attributes->stack_size, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM | SIGCHLD, parameters);
	
	if(pid != 0){
		*tid = pid;
		tcb.tid = pid;
		spin_lock_aquire(&thread_list_lock);
		tlist_insert_end(&tlist_start, tcb);
		spin_lock_release(&thread_list_lock);
		//tlist_display(tlist_start);
	}

	return 0;
}

int thread_create(int* tid, thread_attributes* attributes,int (*fun)(void*), void* parameters ){
	if(attributes == NULL){
		attributes = malloc(sizeof(thread_attributes));
		init_thread_attributes(attributes);
	}
	if(attributes->scheduling_policy == ONE_TO_ONE){
		thread_create_one_one(tid, attributes, fun, parameters);	
	}
	 
}

int thread_join(int tid){
	//printf("joined\n");
	int wstatus;
	waitpid(tid,&wstatus,WUNTRACED);
	
	//tlist_display(tlist_start);
	spin_lock_aquire(&thread_list_lock);
	tlist_delete(&tlist_start,tid);
	spin_lock_release(&thread_list_lock);
	//tlist_display(tlist_start);
	
}

void spin_lock_aquire(spin_lock* sl){
	while(atomic_flag_test_and_set(sl))
		;
}
void spin_lock_release(spin_lock* sl){
	atomic_flag_clear(sl);
}

















































































