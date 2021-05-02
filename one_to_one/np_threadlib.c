#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
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

int thread_create(int* tid, thread_attributes* attributes,int (*fun)(void*), void* parameters ){
	if(attributes == NULL){
		attributes = malloc(sizeof(thread_attributes));
		init_thread_attributes(attributes);
	}
	int pid;
	thread_control_block tcb;
	char* stack = malloc(attributes->stack_size);
	  if (!stack) {
	    perror("malloc error");
	    return 1;
	}
	tcb.stack_ptr = stack;
	pid = clone(*fun, stack + attributes->stack_size, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM | SIGCHLD, parameters);
	
	if(pid != 0){
		*tid = pid;
		tcb.tid = pid;
		spin_lock_aquire(&thread_list_lock);
		tlist_insert_end(&tlist_start, tcb);
		spin_lock_release(&thread_list_lock);
	}
	return 0;
}

int thread_join(int tid, void **retval){
	
	thread_list* thread;
	int wstatus;
	waitpid(tid,&wstatus,WUNTRACED);
	spin_lock_aquire(&thread_list_lock);
	
	thread = tlist_search(tlist_start, tid);
	if(thread  != NULL){
		if(retval != NULL){
			*retval = thread->tcb.return_val;
			
		}
		free(thread->tcb.stack_ptr);
		tlist_delete(&tlist_start, tid);

	spin_lock_release(&thread_list_lock);	
	}

	else{
		spin_lock_release(&thread_list_lock);
		return 1;
	}

	return 0;
}

void thread_exit(void *retval){
	int tid = gettid();
	thread_list* thread;
	spin_lock_aquire(&thread_list_lock);

	thread = tlist_search(tlist_start, tid);
	if(thread  != NULL){
		
		thread->tcb.return_val = retval;
	}

	spin_lock_release(&thread_list_lock);
}

int thread_kill(int tid, int signo){
	int err;
	err = kill(tid,signo);
	return err;
}

void spin_lock_aquire(spin_lock* sl){
	while(atomic_flag_test_and_set(sl))
		;
}
void spin_lock_release(spin_lock* sl){
	atomic_flag_clear(sl);
}

void thread_mutex_lock(mutex_lock* lock){
		while(!atomic_compare_exchange_strong( lock->lock, lock->unlocked, MUTEX_LOCK )){
			syscall(SYS_futex, lock->lock, FUTEX_WAIT, MUTEX_LOCK, NULL, NULL, 0);
		}
}

void thread_mutex_unlock(mutex_lock* lock){

	if(atomic_compare_exchange_strong(lock->lock, lock->locked, MUTEX_UNLOCK)){
		syscall(SYS_futex, lock->lock, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
	}

}

void thread_mutex_init(mutex_lock* lock){
	lock->lock = malloc(sizeof(_Atomic int));
	lock->locked = malloc(sizeof(_Atomic int));	
	lock->unlocked =malloc(sizeof(_Atomic int));
	atomic_store( lock->lock , MUTEX_UNLOCK);
	atomic_store(lock->locked, MUTEX_LOCK);
	atomic_store(lock->unlocked, MUTEX_UNLOCK);
}
















































































