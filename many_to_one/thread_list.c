#include <stdio.h>
#include<stdlib.h>
#include "np_threadlib.h"
#include "thread_list.h"

void tlist_insert_end(thread_list** start, thread_control_block tcb) {
	if (*start == NULL)
	{
		thread_list* new_node = malloc(sizeof(thread_list));
		new_node->tcb = tcb;
		new_node->next = new_node->prev = new_node;
		*start = new_node;
		return;
	}

	thread_list* last = (*start)->prev;
	thread_list* new_node = malloc(sizeof(thread_list));
	new_node->tcb = tcb;

	new_node->next = *start;

	(*start)->prev = new_node;

	new_node->prev = last;

	last->next = new_node;
}

void tlist_insert_begin(thread_list** start, thread_control_block tcb) {
	thread_list *last = (*start)->prev;

	thread_list* new_node = malloc(sizeof(thread_list));
	new_node->tcb = tcb; // Inserting the data

	new_node->next = *start;
	new_node->prev = last;

	last->next = (*start)->prev = new_node;

	*start = new_node;
}


void tlist_display(thread_list* start) {
	if(start == NULL){
		printf("No nodes\n");
		return;
	}
	thread_list *temp = start;

	printf("\nTraversal in forward direction \n");
	while (temp->next != start)
	{
		printf("%d ", temp->tcb.tid);
		temp = temp->next;
	}
	printf("%d ", temp->tcb.tid);
	printf("\nTraversal in reverse direction \n");
	thread_list* last = start->prev;
	temp = last;
	while (temp->prev != last)
	{
		printf("%d ", temp->tcb.tid);
		temp = temp->prev;
	}
	printf("%d ", temp->tcb.tid);
}


void tlist_delete(thread_list** start, int key) {
    if (*start == NULL)
        return;
  
    thread_list *curr = *start, *prev_1 = NULL;
    while (curr->tcb.tid != key) {
        if (curr->next == *start) {
            printf("\nList doesn't have node with value = %d", key);
            return;
        }
  
        prev_1 = curr;
        curr = curr->next;
    }
  
    if (curr->next == *start && prev_1 == NULL) {
        (*start) = NULL;
        free(curr);
        return;
    }
  
    if (curr == *start) {
        prev_1 = (*start)->prev;
  
        *start = (*start)->next;

        prev_1->next = *start;
        (*start)->prev = prev_1;
        free(curr);
    }

    else if (curr->next == *start) {
        prev_1->next = *start;
        (*start)->prev = prev_1;
        free(curr);
    }
    else {
        thread_list* temp = curr->next;
        prev_1->next = temp;
        temp->prev = prev_1;
        free(curr);
    }
}

thread_list* tlist_search(thread_list* start, int tid){
	thread_list *temp = start;
	while (temp->next != start)
	{
		if(temp->tcb.tid == tid){
			return temp;
		}
		temp = temp->next;
	}
	if(temp->tcb.tid == tid){
			return temp;
	}
	return NULL;
}
