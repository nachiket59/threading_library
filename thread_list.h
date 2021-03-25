typedef struct {
	int tid;
}thread_control_block;

typedef struct thread_list{
	int data;
	thread_control_block *tcb;
	struct thread_list *next;
	struct thread_list *prev;
}thread_list;

void tlist_insert_end(thread_list** , thread_control_block * );
void tlist_insert_begin(thread_list** , thread_control_block * );
void tlist_display(thread_list* );
void print_tcb(thread_control_block );
void tlist_delete(struct thread_list** , int );