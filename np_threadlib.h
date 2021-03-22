#define MAX_NO_OF_THREADS 10
#define DETACHABLE 1
#define UNDETACHABLE 0
#define ONE_TO_ONE 1
#define ONE_TO_MENY 2
#define DEFAULT_STACK_SIZE 4096

typedef struct {
	int detachable;
	int scheduling_policy;
	int scheduling_priority;
	//int stack_addr;
	int stack_size;
} thread_attributes;

typedef struct {
	int tid;
}thread_control_block;

typedef struct thread_list{
	int data;
	thread_control_block tcb;
	struct thread_list *next;
	struct thread_list *prev;
}thread_list;

int thread_create(int*, thread_attributes*,int(*)(void*), void* );
void init_thread_attributes(thread_attributes *);