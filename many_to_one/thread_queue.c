#include<stdlib.h>
#include "thread_queue.h"

void tq_append(struct thread_queue** head_ref, int tid){
	struct thread_queue* new_thread_queue = (struct thread_queue*) malloc(sizeof(struct thread_queue));
	struct thread_queue *last = *head_ref; 
	new_thread_queue->tid = tid;
	new_thread_queue->next = NULL;
	if (*head_ref == NULL){
	*head_ref = new_thread_queue;
	return;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = new_thread_queue;
	return;	
}

int tq_pop(struct thread_queue** head){
	int tid;
	thread_queue* tmp = *head;
	if(*head == NULL){
		return -1;
	}
	tid = tmp->tid;
	*head = tmp->next;
	free(tmp);
	return tid;
}