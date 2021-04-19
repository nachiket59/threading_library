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

#define TIMER_SEC 0
#define TIMER_U_SEC 20

#define RUNNABLE 0
#define RETURNED 1
#define STOPPED 2

thread_list* tlist_start = NULL;
spin_lock thread_list_lock;

//signal set fot masking
sigset_t sigmask_set;

//schedular related vars and functions
thread_queue *ready_queue = NULL;
thread_queue *stopped_queue = NULL;

int thread_count = 0;
int cur_tid = 0;
int cont_tid = 0;
int stop_tid = 0;
int term_tid = 0;
ucontext_t* cur_context, sched_context;

void schedular();
void handle_alarm();
void start_timer();
void call_thread(int );
void handle_stop();
void handle_cont();
void handle_term();

void schedular() {
	//ignore alalrm signal 
	signal(SIGVTALRM, SIG_IGN);

	int tid; 
	tid = tq_pop(&ready_queue);
	if(tid == -1){
		return;
	}
	else{

		//printf("tid %d\n",tid );
		thread_list* thread = tlist_search(tlist_start, tid);
		printf("tid: %d state: %d\n",tid, thread->tcb.state );
		while(thread->tcb.state != RUNNABLE){
			if(thread->tcb.state == RETURNED) {
				//printf("state returned %d\n", thread->tcb.state);
				tid = tq_pop(&ready_queue);
				if(tid == -1){
					return;
				}
			}
			if(thread->tcb.state == STOPPED){
				tid = tq_pop(&ready_queue);
				if(tid == -1){
					return;
				}
				tq_append(&ready_queue, thread->tcb.tid);
			}
			thread = tlist_search(tlist_start, tid);
		}

		cur_context = thread->tcb.context;
		
		thread = tlist_search(tlist_start, cur_tid);
		cur_tid = tid;
		tq_append(&ready_queue,thread->tcb.tid);

		//allow the sigalarm
		signal(SIGVTALRM, handle_alarm);
		
		swapcontext(thread->tcb.context, cur_context);
	}
}

void handle_alarm() {
	schedular();
}

void start_timer(){
	struct timeval interval_time;
	interval_time.tv_sec = TIMER_SEC;
	interval_time.tv_usec = TIMER_U_SEC;

	struct itimerval interval;

	interval.it_interval = interval_time;
	interval.it_value = interval_time;

	setitimer(ITIMER_VIRTUAL, &interval, NULL);
}

void call_thread(int tid){
	
	signal(SIGVTALRM, handle_alarm);

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
	signal(SIGVTALRM, handle_alarm);
	signal(SIGTSTP, handle_stop);
	signal(SIGCONT, handle_cont);
	signal(SIGTERM, handle_term);
	start_timer();
}

int thread_create(int* tid, thread_attributes* attributes,void*(*fun)(void*), void* parameters ){
	signal(SIGVTALRM, SIG_IGN);

	if(attributes == NULL) {
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
		tcb.state = RUNNABLE;
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
	    tcb.state = RUNNABLE;

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
	    tcb.state = RUNNABLE;

		tlist_insert_end(&tlist_start, tcb);
		tq_append(&ready_queue, tcb.tid);
		*tid = tcb.tid;
	}
	signal(SIGVTALRM, handle_alarm);
}

int thread_join(int tid){
	thread_list* thread = tlist_search(tlist_start, tid);
	while(thread->tcb.state != RETURNED)
		;
	free(thread->tcb.stack);
	free(thread->tcb.sched_stack);
	free(thread->tcb.sched_context);
	free(thread->tcb.context);
	printf("joined %d\n", tid);
}

void spin_lock_aquire(spin_lock* sl){
	while(atomic_flag_test_and_set(sl))
		;
}
void spin_lock_release(spin_lock* sl){
	atomic_flag_clear(sl);
}

void thread_mutex_lock(mutex_lock* lock){
		while(1){
			if(atomic_compare_exchange_strong( lock->lock, lock->unlocked, MUTEX_LOCK ))
				break;

			schedular();

		}
}

void thread_mutex_unlock(mutex_lock* lock){

	atomic_compare_exchange_strong(lock->lock, lock->locked, MUTEX_UNLOCK);

}

void thread_mutex_init(mutex_lock* lock){
	lock->lock = malloc(sizeof(_Atomic int));
	lock->locked = malloc(sizeof(_Atomic int));	
	lock->unlocked =malloc(sizeof(_Atomic int));
	atomic_store( lock->lock , 0);
	atomic_store(lock->locked, 1);
	atomic_store(lock->unlocked, 0);
}

void handle_stop(){
	//ignore alalrm signal 
	signal(SIGVTALRM, SIG_IGN);
	// printf("int stop\n");
	if(stop_tid == 0){
		printf("in main stop\n");
		signal(SIGVTALRM, handle_alarm);
		kill(getpid(),SIGSTOP);
	}
	else{
		thread_list* thread = tlist_search(tlist_start, stop_tid);
		thread->tcb.state = STOPPED;
		if(stop_tid == cur_tid){
			stop_tid = 0;
			schedular();
		}

	}

	//allow the sigalarm
	signal(SIGVTALRM, handle_alarm);
}

void handle_cont(){
	//ignore alalrm signal 
	signal(SIGVTALRM, SIG_IGN);
	// printf("int cont\n");
	if(cont_tid != 0){
		thread_list* thread = tlist_search(tlist_start, cont_tid);
		thread->tcb.state = RUNNABLE;
	}
	//allow the sigalarm
	signal(SIGVTALRM, handle_alarm);
}

void handle_term(){
	//ignore alalrm signal 
	signal(SIGVTALRM, SIG_IGN);
	// printf("int term\n");
	if(term_tid != 0){
		thread_list* thread = tlist_search(tlist_start, term_tid);
		thread->tcb.state = RETURNED;
		printf("Terminated thread: %d\n",term_tid );
	}
	//allow the sigalarm
	signal(SIGVTALRM, handle_alarm);
}

void thread_kill(int thread_no,int signo){
	if(signo == SIGTSTP){
		stop_tid = thread_no;
		kill(getpid(), SIGTSTP);
		stop_tid = 0;
	}
	if(signo == SIGCONT){
		cont_tid = thread_no;
		kill(getpid(), SIGCONT);
		cont_tid = 0;
	}
	if(signo == SIGTERM || signo == SIGKILL || signo == SIGINT){
		term_tid = thread_no;
		kill(getpid(),SIGTERM);
		term_tid = 0;
	}
}













































































