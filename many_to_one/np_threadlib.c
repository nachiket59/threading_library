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

#define RUNNING 0
#define RETURNED 1


thread_list* tlist_start = NULL;
spin_lock thread_list_lock;

//signal set fot masking
sigset_t sigmask_set;

//schedular related vars and functions
thread_queue *ready_queue = NULL;
int thread_count = 0;
int cur_tid = 0;
ucontext_t* cur_context, sched_context;

void schedular();
void handle_alarm();
void start_timer();
void call_thread(int );

void schedular() {
	int tid; 
	tid = tq_pop(&ready_queue);
	if(tid == -1){
		return;
	}
	else{
		//printf("tid %d\n",tid );
		thread_list* thread = tlist_search(tlist_start, tid);
		//printf("state %d\n", thread->tcb.state);
		while(thread->tcb.state == RETURNED){
			printf("state %d\n", thread->tcb.state);
			tid = tq_pop(&ready_queue);
			if(tid == -1){
				return;
			}
			thread = tlist_search(tlist_start, tid);
		}
		cur_context = thread->tcb.context;
		//thread->tcb.state = RUNNIG;
		thread = tlist_search(tlist_start, cur_tid);
		cur_tid = tid;
		tq_append(&ready_queue,thread->tcb.tid);
		//thread->tcb.state = 4;
		//getcontext(thread->tcb.context);
		swapcontext(thread->tcb.context, cur_context);
	}
}

/*void preemptive_sched(){
	printf("yep\n");
	int tid; 
	tid = tq_pop(&ready_queue);
	printf("%d\n",tid );
	if(tid == -1){
		cur_context = NULL;
	}
	else{
		thread_list* thread = tlist_search(tlist_start, tid);
		cur_context = thread->tcb.context;
		thread->tcb.state = RUNNIG;
		thread = tlist_search(tlist_start, cur_tid);
		cur_tid = tid;
		tq_append(&ready_queue,thread->tcb.tid);
		thread->tcb.state = WAITING;
		//getcontext(thread->tcb.context);
		swapcontext(thread->tcb.context, cur_context);
	}
}*/
void skip(){
	printf("skipped\n");
	exit(0);
	return;
}
void handle_alarm() {
	sigemptyset(&sigmask_set);
	sigaddset(&sigmask_set, SIGALRM);
	sigaddset(&sigmask_set, SIGINT);
	sigprocmask(SIG_BLOCK,&sigmask_set,&sigmask_set);
	schedular();
	sigprocmask(SIG_UNBLOCK,&sigmask_set,&sigmask_set);
	
	/*if(cur_context != NULL){
		 printf("yes\n");
		setcontext(cur_context);
	}*/
}

void start_timer(){
	struct timeval interval_time;
	interval_time.tv_sec = 2;
	interval_time.tv_usec = 0;

	struct itimerval interval;

	interval.it_interval = interval_time;
	interval.it_value = interval_time;

	setitimer(ITIMER_REAL, &interval, NULL);
}

void call_thread(int tid){
	thread_list* thread = tlist_search(tlist_start, tid);
	if(thread != NULL){
		thread->tcb.return_val = (thread->tcb.start_routine)(thread->tcb.params);
		thread->tcb.state = RETURNED;
		//printf("returned %d\n",thread->tcb.state);
		//sleep(1);
	}
}


void init_thread_attributes(thread_attributes * attributes){
	attributes->detachable = DETACHABLE;
	attributes->scheduling_policy = ONE_TO_ONE;
	attributes->stack_size = DEFAULT_STACK_SIZE;
	return; 
}

int thread_library_init(){
	signal(SIGALRM, handle_alarm);
	signal(SIGINT, skip);
	start_timer();
}
void dummy(){
	printf("in dummy\n");
}
int thread_create(int* tid, thread_attributes* attributes,void*(*fun)(void*), void* parameters ){
	
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
		tcb.state = RUNNING;
		getcontext(tcb.context);
		tlist_insert_end(&tlist_start, tcb);
		

		thread_count += 1; 
		tcb.tid = thread_count;
		
		tcb.context = malloc(sizeof(ucontext_t));
		getcontext(tcb.context);
		tcb.sched_context = malloc(sizeof(ucontext_t));
		getcontext(tcb.sched_context);

		// allocate stack
		tcb.stack = malloc(attributes->stack_size);
		tcb.sched_stack = malloc(attributes->stack_size);
		
		//Make inittal context
		tcb.sched_context->uc_stack.ss_sp = tcb.sched_stack;
	    tcb.sched_context->uc_stack.ss_size = attributes->stack_size;
	    tcb.sched_context->uc_link = NULL;
	    makecontext(tcb.sched_context, schedular,0);

		tcb.context->uc_stack.ss_sp = tcb.stack;
	    tcb.context->uc_stack.ss_size = attributes->stack_size;
	    tcb.context->uc_link = tcb.sched_context;
	    makecontext(tcb.context, call_thread,1, tcb.tid);
		
	    // save pointer of actual routine to call from the wrapper function call_threads
	    tcb.start_routine = fun;
	    tcb.params = parameters;
	    tcb.return_val = NULL;
	    tcb.state = RUNNING;

		tlist_insert_end(&tlist_start, tcb);
		tq_append(&ready_queue, tcb.tid);
		*tid = tcb.tid;
	}
	else {
		thread_count += 1; 
		tcb.tid = thread_count;
		
		tcb.context = malloc(sizeof(ucontext_t));
		getcontext(tcb.context);
		tcb.sched_context = malloc(sizeof(ucontext_t));
		getcontext(tcb.sched_context);

		//Allocate stack
		tcb.stack = malloc(attributes->stack_size);
		tcb.sched_stack = malloc(attributes->stack_size);

		//Make initial context for a thread
		tcb.sched_context->uc_stack.ss_sp = tcb.sched_stack;
	    tcb.sched_context->uc_stack.ss_size = attributes->stack_size;
	    tcb.sched_context->uc_link = NULL;
	    makecontext(tcb.sched_context, schedular,0);

		tcb.context->uc_stack.ss_sp = tcb.stack;
	    tcb.context->uc_stack.ss_size = attributes->stack_size;
	    tcb.context->uc_link = tcb.sched_context;
	    makecontext(tcb.context,call_thread,1,tcb.tid);
		
		// save pointer of actual routine to call from the wrapper function call_threads
	    tcb.start_routine = fun;
	    tcb.params = parameters;
	    tcb.return_val = NULL;
	    tcb.state = RUNNING;

		tlist_insert_end(&tlist_start, tcb);
		tq_append(&ready_queue, tcb.tid);
		*tid = tcb.tid;
	}
}

int thread_join(int tid){
	thread_list* thread = tlist_search(tlist_start, tid);
	while(thread->tcb.state != RETURNED)
		;
	printf("joined %d\n", tid);
}

void spin_lock_aquire(spin_lock* sl){
	while(atomic_flag_test_and_set(sl))
		;
}
void spin_lock_release(spin_lock* sl){
	atomic_flag_clear(sl);
}

















































































