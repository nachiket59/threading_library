#include <stdatomic.h>

#define MAX_NO_OF_THREADS 10

/*thread_attributes*/
#define DETACHABLE 1
#define UNDETACHABLE 0
#define ONE_TO_ONE 1
#define ONE_TO_MENY 2
#define DEFAULT_STACK_SIZE 4096

/*spin locks*/
#define SPIN_LOCK_LOCKED 0
#define SPIN_LOCK_UNLOCKED 1

typedef atomic_flag spin_lock;

/*mutex locks*/
#define MUTEX_LOCK 0
#define MUTEX_UNLOCK 1
typedef struct {
	_Atomic int *lock;
	_Atomic int *locked;
	_Atomic int *unlocked;
}mutex_lock;

typedef struct {
	int detachable;
	int scheduling_policy;
	int scheduling_priority;
	//int stack_addr;
	int stack_size;
} thread_attributes;

int thread_create(int*, thread_attributes*,int(*)(void*), void* );

void init_thread_attributes(thread_attributes *);

int thread_join(int);

int spin_lock_init(spin_lock* );

void spin_lock_aquire(spin_lock* );

void spin_lock_release(spin_lock* );

void spin_lock_destroy(spin_lock* );	

void thread_mutex_lock(mutex_lock* );
void thread_mutex_unlock(mutex_lock* );
void thread_mutex_init(mutex_lock* );