typedef struct {
	int tid;
	void *stack_ptr;
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

