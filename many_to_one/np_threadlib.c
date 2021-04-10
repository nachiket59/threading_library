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
#include <time.h>
#include <sys/time.h>
#include "np_threadlib.h"
#include "thread_list.h"
#include "thread_queue.h"

thread_list* tlist_start = NULL;

thread_queue *ready_queue = NULL;

int thread_count = 0;

spin_lock thread_list_lock;

int cur_tid = 0;
ucontext_t* cur_context;

void schedular() {
	int tid;
	tid = tq_pop(&ready_queue);
	if(tid == -1){
		cur_context = NULL;
	}
	else{
		thread_list* thread = tlist_search(tlist_start, tid);
		cur_context = thread->tcb.context;
		thread = tlist_search(tlist_start, cur_tid);
		cur_tid = tid;
		tq_append(&ready_queue,thread->tcb.tid);
		getcontext(thread->tcb.context);
	}	
}

void handle_alarm() {
	schedular();
	if(cur_context != NULL){
		setcontext(cur_context);
	}
}

void init_thread_attributes(thread_attributes * attributes){
	attributes->detachable = DETACHABLE;
	attributes->scheduling_policy = ONE_TO_ONE;
	attributes->stack_size = DEFAULT_STACK_SIZE;
	return; 
}

void start_timer(){
	struct timeval interval_time;
	interval_time.tv_sec = 1;
	interval_time.tv_usec = 0;

	struct itimerval interval;

	interval.it_interval = interval_time;
	interval.it_value = interval_time;

	setitimer(ITIMER_REAL, &interval, NULL);
}

int thread_library_init(){
	signal(SIGALRM, handle_alarm);
	start_timer();
}


int thread_create(int* tid, thread_attributes* attributes,void (*fun)(), void* parameters ){
	
	if(attributes == NULL){
		attributes = malloc(sizeof(thread_attributes));
		init_thread_attributes(attributes);
	}	

	thread_control_block tcb;
	void *stack;
	if(thread_count == 0) { 
		start_timer();
		// save the main thread context
		tcb.tid = thread_count;
		tcb.context = malloc(sizeof(ucontext_t));
		getcontext(tcb.context);
		tlist_insert_end(&tlist_start, tcb);
		tq_append(&ready_queue, tcb.tid);
		

		thread_count += 1; 
		tcb.tid = thread_count;
		
		tcb.context = malloc(sizeof(ucontext_t));
		getcontext(tcb.context);
		tcb.stack = malloc(attributes->stack_size);
		tcb.context->uc_stack.ss_sp = tcb.stack;
	    tcb.context->uc_stack.ss_size = attributes->stack_size;
	    tcb.context->uc_link = NULL;
	    makecontext(tcb.context, fun, 0);
		
		tlist_insert_end(&tlist_start, tcb);
		tq_append(&ready_queue, tcb.tid);
	}
	else {
		thread_count += 1; 
		tcb.tid = thread_count;
		
		tcb.context = malloc(sizeof(ucontext_t));
		getcontext(tcb.context);
		tcb.stack = malloc(attributes->stack_size);
		tcb.context->uc_stack.ss_sp = tcb.stack;
	    tcb.context->uc_stack.ss_size = attributes->stack_size;
	    tcb.context->uc_link = NULL;
	    makecontext(tcb.context, fun, 0);
		
		tlist_insert_end(&tlist_start, tcb);
		tq_append(&ready_queue, tcb.tid);
	}
}

int thread_join(int tid){
	thread_list* thread;
	int wstatus;
	waitpid(tid,&wstatus,WUNTRACED);

	spin_lock_aquire(&thread_list_lock);
	
	thread = tlist_search(tlist_start, tid);
	if(thread  != NULL){
		free(thread->tcb.stack);
		tlist_delete(&tlist_start, tid);

	spin_lock_release(&thread_list_lock);	
	}

	else{
		return 1;
	}

	return 0;
}

void spin_lock_aquire(spin_lock* sl){
	while(atomic_flag_test_and_set(sl))
		;
}
void spin_lock_release(spin_lock* sl){
	atomic_flag_clear(sl);
}

















































































