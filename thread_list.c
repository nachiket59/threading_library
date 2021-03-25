// Structure of a Node
#include <stdio.h>
#include<stdlib.h>
#include "np_threadlib.h"
#include "thread_list.h"



// Function to insert at the end
void tlist_insert_end(thread_list** start, thread_control_block* tcb)
{
	// If the list is empty, create a single node
	// circular and doubly list
	if (*start == NULL)
	{
		thread_list* new_node = malloc(sizeof(thread_list));
		new_node->tcb = tcb;
		new_node->next = new_node->prev = new_node;
		*start = new_node;
		return;
	}

	// If list is not empty

	/* Find last thread_list*/
	thread_list* last = (*start)->prev;

	// Create Node dynamically
	thread_list* new_node = malloc(sizeof(thread_list));
	new_node->tcb = tcb;

	// Start is going to be next of new_node
	new_node->next = *start;

	// Make malloc(sizeof(thread_list)) previous of start
	(*start)->prev = new_node;

	// Make last preivous of malloc(sizeof(thread_list))
	new_node->prev = last;

	// Make malloc(sizeof(thread_list)) next of old last
	last->next = new_node;
}

// Function to insert Node at the beginning
// of the List,
void tlist_insert_begin(thread_list** start, thread_control_block* tcb)
{
	// Pointer points to last Node
	thread_list *last = (*start)->prev;

	thread_list* new_node = malloc(sizeof(thread_list));
	new_node->tcb = tcb; // Inserting the data

	// setting up previous and next of malloc(sizeof(thread_list))
	new_node->next = *start;
	new_node->prev = last;

	// Update next and previous pointers of start
	// and last.
	last->next = (*start)->prev = new_node;

	// Update start pointer
	*start = new_node;
}


void tlist_display(thread_list* start)
{
	thread_list *temp = start;

	printf("\nTraversal in forward direction \n");
	while (temp->next != start)
	{
		print_tcb(*(temp->tcb));
		temp = temp->next;
	}
	print_tcb(*(temp->tcb));

	printf("\nTraversal in reverse direction \n");
	thread_list* last = start->prev;
	temp = last;
	while (temp->prev != last)
	{
		print_tcb(*(temp->tcb));
		temp = temp->prev;
	}
	print_tcb(*(temp->tcb));
}

void tlist_delete(thread_list** start, int tid)
{
    // If list is empty
    if (*start == NULL)
        return;
  
    // Find the required node
    // Declare two pointers and initialize them
    thread_list *curr = *start, *prev_1 = NULL;
    while (curr->tcb->tid != tid) {
    	printf("%d %d\n",curr->tcb->tid,tid );
        // If node is not present in the list
        if (curr->next == *start) {
            printf("\nList doesn't have node with value = %d", tid);
            return;
        }
        prev_1 = curr;
        curr = curr->next;
    }
    // Check if node is the only node in list
    if (curr->next == *start && prev_1 == NULL) {
        (*start) = NULL;
        free(curr);
        return;
    }
  
    // If list has more than one node,
    // check if it is the first node
    if (curr == *start) {
        // Move prev_1 to last node
        prev_1 = (*start)->prev;
  
        // Move start ahead
        *start = (*start)->next;
  
        // Adjust the pointers of prev_1 and start node
        prev_1->next = *start;
        (*start)->prev = prev_1;
        free(curr);
    }
  
    // check if it is the last node
    else if (curr->next == *start) {
        // Adjust the pointers of prev_1 and start node
        prev_1->next = *start;
        (*start)->prev = prev_1;
        free(curr);
    }
    else {
        // create new pointer, points to next of curr node
        thread_list* temp = curr->next;
  
        // Adjust the pointers of prev_1 and temp node
        prev_1->next = temp;
        temp->prev = prev_1;
        free(curr);
    }
}


void print_tcb(thread_control_block tcb){
	printf("tid %d\n", tcb.tid);
}


/*
int main()
{
	
	thread_list* start = NULL;

	// Insert 5. So linked list becomes 5->NULL
	insertEnd(&start, 5);

	// Insert 4 at the beginning. So linked 
	// list becomes 4->5
	insertBegin(&start, 4);

	// Insert 7 at the end. So linked list
	// becomes 4->5->7
	insertEnd(&start, 7);

	// Insert 8 at the end. So linked list 
	// becomes 4->5->7->8
	insertEnd(&start, 8);

	// Insert 6, after 5. So linked list 
	// becomes 4->5->6->7->8
	insertAfter(&start, 6, 5);

	printf("Created circular doubly linked list is: ");
	display(start);

	return 0;
}*/
