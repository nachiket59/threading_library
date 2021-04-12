#include <ucontext.h>
typedef struct {
	int tid;
	void *stack;
	void *sched_stack;
	void *(*start_routine) (void *);
	void *params;
	void *return_val;
	int state;
	ucontext_t *sched_context;
	ucontext_t *context;
}thread_control_block;

struct thread_list{
	int data;
	thread_control_block tcb;
	struct thread_list *next;
	struct thread_list *prev;
};
typedef struct thread_list thread_list;

void tlist_insert_end(thread_list** , thread_control_block );
void tlist_insert_begin(thread_list** , thread_control_block );
void tlist_display(thread_list* start);
void tlist_delete(thread_list** start, int key);
thread_list* tlist_search(thread_list* start, int tid);

