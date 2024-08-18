#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"


/*
 * Define your call struct, and call center struct here.
 */
struct call {
	// Defines the members of the call struct
	int id;
	char name[100];
	char reason[100];
};

struct stack{
	// Defines the stack struct
	struct list* list;
};

struct queue{
	// Defines the queue struct
	struct dynarray* array;
};
void init_call_center(struct stack** call_stack,struct queue** call_queue){
	// Initilizes the stack and queue using the respective create functions
	*call_stack = stack_create();
	*call_queue = queue_create();
}

void free_call_center(struct stack** call_stack, struct queue** call_queue){
	// Logic to free stack when the stack isnt empty
	while(!stack_isempty(*call_stack)){
		// Uses stack_pop to set the top of the array to call
		struct call* call = (struct call*)stack_pop(*call_stack);
		// If call isnt null, frees call
		if(call != NULL){
			free(call);
		}
	}
	// Logic to free queue when the queue isnt empty
	while(!queue_isempty(*call_queue)){
		// Uses dequeue to set the front of the queue to call
		struct call* call = (struct call*)queue_dequeue(*call_queue);
		// If call isnt null, frees call
		if(call != NULL){
			free(call);
		}
	}
	// Frees the stack and queue initilizes in init_call_center
	stack_free(*call_stack);
	queue_free(*call_queue);
}

void receive_call(struct stack* call_stack, struct queue* call_queue, int* size, int* queue_size){
	// Allocates memory for call
	struct call* call = malloc(sizeof(struct call));
	// Reads data from the user of call details
	printf("Enter caller's name: \n");
	scanf("%s", call->name);
	printf("Enter caller's reason: \n");
	scanf("%s", call->reason);
	// Uses enqueue to add the call to the queue
	queue_enqueue(call_queue, call);
	printf("The call has been successfully added to the queue!\n");
	// Sets the call ID +1 of the previous call, initalizes in main() as 0
	call->id = *size + 1;
	// Increases size by 1 for next call
	(*size)++;
	// Increases size of queue in order to keep track for print_queue
	(*queue_size)++;
}

void answer_call(struct stack* call_stack, struct queue* call_queue, int* call_size, int* queue_size){
	// Checks if the queue is empty, returns
	if(*queue_size == 0){
		printf("No more calls need to be answered at the moment!\n");
		return;
	}
	// Dequeues the queue, setting call as the front of the queue
	struct call* call = (struct call*)queue_dequeue(call_queue);
	// Adds the deqeued call to the top of the stack
	stack_push(call_stack, call);
	// Print the values of call, or the front of the queue, the value of the call answered
	printf("The following call has been answered and added to the stack!\n");
	printf("Caller's ID: %d \n", call->id);
	printf("Caller's name: %s \n", call->name);
	printf("Caller's reason: %s \n", call->reason);
	// Increases the amount of calls taken by 1
	(*call_size)++;
	// Decreases the amount of calls in the queue by 1
	(*queue_size)--;
}

void print_stack(struct stack* call_stack, struct queue* call_queue, int* call_size){
	// Returns call size, or the amount of calls taken
	printf("Number of calls answered: %d \n", *call_size);
	// Uses the stack_top function to get the top of the stack
	void* top = stack_top(call_stack);
	// Sets call as the top of the stack, which is the last call dequeued, and the latest call taken
	struct call* call = (struct call*)top;
	// Checks if call is empty, returns
	if(call == NULL){
		printf("No calls have been answered!\n");
		return;
	}
	// Prints the values of the last call answered, or the top of the stack
	printf("Details of the last call answered: \n");
	printf("Caller's ID: %d \n", call->id);
	printf("Caller's name: %s \n", call->name);
	printf("Caller's reason: %s \n", call->reason);
}

void print_queue(struct stack* call_stack, struct queue* call_queue, int* call_size, int* queue_size){
	// Returns the queue siz
	printf("Number of calls to be answered: %d \n", *queue_size);
	// Checks if queue is empty, returns
	if(*queue_size == 0){
		return;
	}
	// Uses queue_front to get the top of the queue
	void* front = queue_front(call_queue);
	// Sets call as the front of the queue, or the next call that will be answered
	struct call* call = (struct call*)front;
	// Checks if there are no more calls, returns
	if(call == NULL){
		printf("No calls have been answered!\n");
		return;
	}
	// Prints the values of the next call to be answered, or the front of the queue
	printf("Details of the first call answered: \n");
	printf("Caller's ID: %d \n", call->id);
	printf("Caller's name: %s \n", call->name);
	printf("Caller's reason: %s \n", call->reason);
}

int main(int argc, char *argv[]) {
	// Inits three size variables
	// This variable keeps track of ID index, only ever increases
	int size = 0;
	// This variable keeps in track of calls taken, only ever increases
	int call_size = 0;
	// This variable keeps track of the size of the queue, can increase or decrease based on user calls
	int queue_size = 0;
	// Defines call_stack and call_queue
	struct stack* call_stack;
	struct queue* call_queue;
	int choice;
	// Calls init_call_center, initlizing the stack and queue
	init_call_center(&call_stack,&call_queue);
	// Switch case for each option presented
	do{
		printf("1. Receive a new call\n");
		printf("2. Answer a new call\n");
		printf("3. Current state of the stack - answered calls\n");
		printf("4. Current state of the queue - calls to be answered\n");
		printf("5. Quit\n");
		printf("Choose an option: \n");
		scanf("%d", &choice);
		switch(choice){
			case 1:
				receive_call(call_stack, call_queue, &size, &queue_size);
				break;
			case 2:
				answer_call(call_stack, call_queue, &call_size, &queue_size);
				break;
			case 3:
				print_stack(call_stack, call_queue, &call_size);
				break;
			case 4:
				print_queue(call_stack, call_queue, &call_size, &queue_size);
				break;
			case 5:
				printf("Thank you! Have a nice day!");
				// Calls free_call_center when user quits, cleanning all mallocs
				free_call_center(&call_stack, &call_queue);
				return 0;
			default:
				printf("Invalid option.\n");
		}
	}while(choice != 5);
}
