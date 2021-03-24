// Structure of a Node
#include <stdio.h>
#include<stdlib.h>
#include "thread_list.h"

// Function to insert at the end
void tlist_insert_end(thread_list** start, thread_control_block tcb)
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
void tlist_insert_begin(thread_list** start, thread_control_block tcb)
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
		printf("%d ", temp->data);
		temp = temp->next;
	}
	printf("%d ", temp->data);

	printf("\nTraversal in reverse direction \n");
	thread_list* last = start->prev;
	temp = last;
	while (temp->prev != last)
	{
		printf("%d ", temp->data);
		temp = temp->prev;
	}
	printf("%d ", temp->data);
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
