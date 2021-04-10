

typedef struct thread_queue {
  int tid;
  struct thread_queue *next;
}thread_queue;

void tq_append(struct thread_queue** head_ref, int tid);
int tq_pop(struct thread_queue** head);

