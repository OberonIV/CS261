/*
 * This file is where you should implement your queue-from-stacks.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Carl Jeffri Resurreccion
 * Email: resurrec@oregonstate.edu
 */

#include <stdlib.h>

#include "stack.h"
#include "queue_from_stacks.h"

/*
 * This is the structure that will be used to represent a queue using two
 * stacks.  It specifically contains two fields, each representing a stack.
 * These two stacks should be used together as the underlying data storage for
 * the queue.
 *
 * You should not modify this structure.
 */
struct queue_from_stacks {
  struct stack* s1;
  struct stack* s2;
};

/*
 * This function should allocate and initialize a new, empty queue-from-stacks
 * and return a pointer to it.
 */
struct queue_from_stacks* queue_from_stacks_create() {
	// Allocates memory for both stacks
  	struct queue_from_stacks* qfs = malloc(sizeof(struct queue_from_stacks));
	// Initalizes stacks using stack_create
	qfs->s1 = stack_create();
	qfs->s2 = stack_create();
  	return qfs;
}

/*
 * This function should free the memory associated with a queue-from-stacks.
 * While this function should up all memory used in the queue-from-stacks
 * itself, it should not free any memory allocated to the pointer values stored
 * in the queue.  This is the responsibility of the caller.
 *
 * Params:
 *   qfs - the queue-from-stacks to be destroyed.  May not be NULL.
 */
void queue_from_stacks_free(struct queue_from_stacks* qfs) {
	// Free data in each stack
	stack_free(qfs->s1);
	stack_free(qfs->s2);
	// Frees qufs
	free(qfs);
  	return;
}

/*
 * This function should indicate whether a given queue-from-stacks is currently
 * empty.  Specifically, it should return 1 if the specified queue-from-stacks
 * is empty (i.e. contains no elements) and 0 otherwise.
 *
 * Params:
 *   qfs - the queue-from-stacks whose emptiness is being questioned.  May not
 *     be NULL.
 */
int queue_from_stacks_isempty(struct queue_from_stacks* qfs) {
	// Returns true if both are empty, false if not
	return stack_isempty(qfs->s1) && stack_isempty(qfs->s2);
}

/*
 * This function should enqueue a new value into a given queue-from-stacks.
 * The value to be enqueued is specified as a void pointer.
 *
 * Params:
 *   qfs - the queue-from-stacks into which a value is to be enqueued.  May not
 *     be NULL.
 *   val - the value to be enqueued.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void queue_from_stacks_enqueue(struct queue_from_stacks* qfs, void* val) {
	//Pushes value into stack 1, my "inbox"
	stack_push(qfs->s1, val);
  	return;
}

/*
 * This function should return the value stored at the front of a given
 * queue-from-stacks *without* removing that value.
 *
 * Params:
 *   qfs - the queue-from-stacks from which to query the front value.  May not be NULL.
 */
void* queue_from_stacks_front(struct queue_from_stacks* qfs) {
	// Checks if stack 2 is empty and stack 1 is full
	if(stack_isempty(qfs->s2)){
		while(!stack_isempty(qfs->s1)){
			// Pops from stack one, the "inbox"
			void* front = stack_pop(qfs->s1);
			// Pushes to stack two, the "outbox"
			stack_push(qfs->s2, front);
		}
	}	
	// Returns the top value of stack two
  	return stack_top(qfs->s2);
	
	// This effectively reverses the order of list, so when the list is dequeued, it starts from the "end"

	// Logic Check
	// Enqueued in order (1, 2, 3, 4, 5)
	// s1 = [5 4 3 2 1]
	// s2 = [x x x x x]
	// queue_from_stacks_front
	// s1 = [x x x x x]
	// s2 = [1 2 3 4 5]
	// Pops top of stack 2, which was in the order of original data, making this a queue
	// One thing I do not know is what will happen if s2, the "outbox" has data already inside
}

/*
 * This function should dequeue a value from a given queue-from-stacks and
 * return the dequeued value.
 *
 * Params:
 *   qfs - the queue-from-stacks from which a value is to be dequeued.  May not
 *     be NULL.
 *
 * Return:
 *   This function should return the value that was dequeued.
 */
void* queue_from_stacks_dequeue(struct queue_from_stacks* qfs) {
	// Uses queue_from_stack_front to reverse list
	queue_from_stacks_front(qfs);
	// Pops from stack two
	return stack_pop(qfs->s2);
}