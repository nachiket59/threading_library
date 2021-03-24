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

thread_list *tlist_start = NULL;

void *safe_malloc(size_t n)
{
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
        abort();
    }
    return p;
}

void init_thread_attributes(thread_attributes * attributes){
	attributes->detachable = DETACHABLE;
	attributes->scheduling_policy = ONE_TO_ONE;
	attributes->stack_size = DEFAULT_STACK_SIZE;
	return; 
}


int thread_create(int* tid, thread_attributes* attributes,int (*fun)(void*), void* parameters ){
	int pid;
	thread_control_block tcb;
	if(attributes == NULL){
		attributes = malloc(sizeof(thread_attributes));
		init_thread_attributes(attributes);
	}

	void * stack = safe_malloc(attributes->stack_size);

	pid = clone(*fun, stack, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM | SIGCHLD, parameters);
	
	if(pid != 0){
		*tid = pid;
		tcb.tid = pid;
		tlist_insert_end(&tlist_start, tcb);
	}

	return 1; 
}

int thread_join(int tid){
	int wstatus;
	waitpid(tid,&wstatus,WUNTRACED);	
	return wstatus;
}


















































































