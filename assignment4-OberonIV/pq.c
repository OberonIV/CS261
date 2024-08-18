/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Carl Jeffri Resurreccion
 * Email: resurrec@oregonstate.edu
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
	struct dynarray* array;
};
// Struct that represents elements in the priority queue. 
struct pq_element{
	void* value;
	int priority;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	// Allocates memory for priority queue
	struct pq* pq = malloc(sizeof(struct pq));
	// Initializes dynarray
	pq->array = dynarray_create();
	return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue. That is the responsibility of the caller. However, you
 * do need to free the memory that holds each element stored in the priority 
 * queue. 
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	// Frees each element in the dynarray
	for(int i = 0; i < dynarray_size(pq->array); i++){
		free(dynarray_get(pq->array, i));
	}
	// Frees the dynamic array
	dynarray_free(pq->array);
	// Frees the queue
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	// Returns 1 if priority queue is empty
	return dynarray_size(pq->array) == 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	// Creates a new element
	struct pq_element* element = malloc(sizeof(struct pq_element));
	// Initializes the element with wanted value and priority
	element->value = value;
	element->priority = priority;
	// Inserts element into array
	dynarray_insert(pq->array, element);
	// Sets the index for new element at the end of the dynarray
	int index = dynarray_size(pq->array) - 1;
	// Uses pseudocode from lexture 18
	while(index > 0){
		// Sets parent element (i - 1) / 2
		int parent_index = (index - 1) / 2;
		// Gets parent and current elements for comparison
		struct pq_element* parent = dynarray_get(pq->array, parent_index);
		struct pq_element* current = dynarray_get(pq->array, index);
		// Compares inserted element with parent
		if(current->priority < parent->priority){
			// Swaps the elements if priority of current is higher than parent
			dynarray_set(pq->array, index, parent);
			dynarray_set(pq->array, parent_index, current);
			index = parent_index;
		}
		else{
			// Breaks out of loop if parent priority is greater than current
			break;
		}
	}
	return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	// Returns NULL if priority queue is empty
	if(pq_isempty(pq)){
		return NULL;
	}
	// Sets element as first element in dynarray
	struct pq_element* element = dynarray_get(pq->array, 0);
	// Returns value
	return element->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	// Returns -1 if priority queue is empty (cant return NULL)
	if(pq_isempty(pq)){
		return -1; 
	}
	// Sets element as first element in dynarray
	struct pq_element* element = dynarray_get(pq->array, 0);
	// Returns priority
	return element->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
 
 
 // Uses pseudocode from lecture 18
 
 // Seperate function for comparison so remove_first is short
void heap_compare(struct pq* pq, int index){
	// Sets size to the size of the dynarray
	int size = dynarray_size(pq->array);
	while(1){
		// Computes indices of the children of the replacement element
		int left_child_index = 2 * index + 1;
		int right_child_index = 2 * index + 2;
		int smallest_index = index;
		
		// Sees if left child is smaller than element
		if(left_child_index < size){
			struct pq_element* left_child = dynarray_get(pq->array, left_child_index);
			struct pq_element* smallest = dynarray_get(pq->array, smallest_index);
			if(left_child->priority < smallest->priority){
				// Sets smallest index to left child
				smallest_index = left_child_index;
			}
		}
		// Sees if right child is smaller than element
		if(right_child_index < size){
			struct pq_element* right_child = dynarray_get(pq->array, right_child_index);
			struct pq_element* smallest = dynarray_get(pq->array, smallest_index);
			if(right_child->priority < smallest->priority){
				// Sets the smallest index to right child
				smallest_index = right_child_index;
			}
		}
		if(smallest_index == index){
			break;
		}
		// Creates temp element, then swaps the elements
		struct pq_element* temp = dynarray_get(pq->array, index);
		dynarray_set(pq->array, index, dynarray_get(pq->array, smallest_index));
		dynarray_set(pq->array, smallest_index, temp);
		index = smallest_index;
	}
}

void* pq_remove_first(struct pq* pq) {
	// Sets the size of dynarray
	int size = dynarray_size(pq->array);
	if(size == 0){
		return NULL;
	}
	
	// Remembers the first element
	// Sets first element to root
	struct pq_element* first_element = dynarray_get(pq->array, 0);
	// Sets first value to the value
	void* first_value = first_element->value;
	free(first_element);
	
	if(size == 1){
		dynarray_remove(pq->array, 0);
		return first_value;
	}
	// Sets last element to last element in dynarray
	struct pq_element* last_element = dynarray_get(pq->array, size - 1);
	// Sets root to last element
	dynarray_set(pq->array, 0 , last_element);
	// Removes last element
	dynarray_remove(pq->array, size - 1);
	
	heap_compare(pq, 0);
	// Returns the value to be removed
	return first_value;
}
