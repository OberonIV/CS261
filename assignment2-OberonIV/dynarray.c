/*
 * This file contains a simple implementation of a dynamic array.  See the
 * documentation below for more information on the individual functions in
 * this implementation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dynarray.h"

/*
 * This structure is used to represent a single dynamic array.
 */
struct dynarray {
  void** data;
  int size;
  int capacity; 
  // Struct members to keep track of circular buffer
  int head;
  int tail;
};

#define DYNARRAY_INIT_CAPACITY 4

/*
 * This function allocates and initializes a new, empty dynamic array and
 * returns a pointer to it.
 */
struct dynarray* dynarray_create() {
  struct dynarray* da = malloc(sizeof(struct dynarray));
  assert(da);

  da->data = malloc(DYNARRAY_INIT_CAPACITY * sizeof(void*));
  assert(da->data);
  da->size = 0;
  da->capacity = DYNARRAY_INIT_CAPACITY;
  // Initializes head and tail to be zero, to keep track of circular buffer
  da->head = 0;
  da->tail = 0;

  return da;
}

/*
 * This function frees the memory associated with a dynamic array.  Freeing
 * any memory associated with values stored in the array is the responsibility
 * of the caller.
 *
 * Params:
 *   da - the dynamic array to be destroyed.  May not be NULL.
 */
void dynarray_free(struct dynarray* da) {
  assert(da);
  free(da->data);
  free(da);
}

/*
 * This function returns the size of a given dynamic array (i.e. the number of
 * elements stored in it, not the capacity).
 */
int dynarray_size(struct dynarray* da) {
  assert(da);
  return da->size;
}


/*
 * Auxilliary function to perform a resize on a dynamic array's underlying
 * storage array.
 */
void _dynarray_resize(struct dynarray* da, int new_capacity) {
  assert(new_capacity > da->size);

  /*
   * Allocate space for the new array.
   */
  void** new_data = malloc(new_capacity * sizeof(void*));
  assert(new_data);

  /*
   * Initialize every element to NULL
   */
  for (int i = 0; i < new_capacity; i++) {
    new_data[i] = NULL;
  }

  /*
   * Copy data from the old array to the new one.
   */
   // Sets index to head
  int idx = da->head;
  for (int i = 0; i < da->size; i++) {
    new_data[i] = da->data[idx];
	// Increments through idx, if idx is at the end, it will mod to 0, wrapping around to start
	idx = (idx + 1) % da->capacity;
  }

  /*
   * Put the new array into the dynarray struct.
   */
  free(da->data);
  da->data = new_data;
  da->capacity = new_capacity;
  //Sets head to 0, tail to size
  da->head = 0;
  da->tail = da->size;
}

/*
 * This function inserts a new value to a given dynamic array.  The new element
 * is always inserted at the *end* of the array.
 *
 * Params:
 *   da - the dynamic array into which to insert an element.  May not be NULL.
 *   val - the value to be inserted.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_insert(struct dynarray* da, void* val) {
  assert(da);

  /*
   * Make sure we have enough space for the new element.  Resize if needed.
   */
  if (da->size == da->capacity) {
    _dynarray_resize(da, 2 * da->capacity);
  }

  /*
   * Put the new element at the end of the array.
   */
  //Uses da->tail instead of da->size, as da->tail = da->size
  da->data[da->tail] = val;
  // increments tail by 1, checking if it needs to wrap arround
  da->tail = (da->tail + 1) % da->capacity;
  da->size++;
}

/*
 * This function removes an element at a specified index from a dynamic array.
 * All existing elements following the specified index are moved forward to
 * fill in the gap left by the removed element.
 *
 * Params:
 *   da - the dynamic array from which to remove an element.  May not be NULL.
 *   idx - the index of the element to be removed.  The value of `idx` must be
 *     between 0 (inclusive) and n (exclusive), where n is the number of
 *     elements stored in the array.
 */
void dynarray_remove(struct dynarray* da, int idx) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  /*
   * Move all elements behind the one being removed forward one index,
   * overwriting the element to be removed in the process.
   */
   // Logic checks, seeing if it needs to wrap around
  int val = (da->head + idx) % da->capacity;
  da->data[val] = NULL;
  if(idx == 0){
	  da->head = (da->head + 1) % da->capacity;
  }
  da->size--;
}

/*
 * This function returns the value of an existing element in a dynamic array.
 *
 * Params:
 *   da - the dynamic array from which to get a value.  May not be NULL.
 *   idx - the index of the element whose value should be returned.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 */
void* dynarray_get(struct dynarray* da, int idx) {
  assert(da);
  assert(idx < da->size && idx >= 0);
  int val = (da->head + idx) % da->capacity;
  return da->data[val];
}

/*
 * This function updates (i.e. overwrites) the value of an existing element in
 * a dynamic array.
 *
 * Params:
 *   da - the dynamic array in which to set a value.  May not be NULL.
 *   idx - the index of the element whose value should be updated.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 *   val - the new value to be set.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_set(struct dynarray* da, int idx, void* val) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  da->data[idx] = val;
}


/*
 * helper function for testing, do not modify
 */

/*
 * This function prints out all spots (from 0 to capacity) in a dynamic array.
 * If the spot does not have an element, print NULL.
 *
 * Params:
 *   da - the dynamic array from which to print.  May not be NULL.
 *   void (*p) (void* a) - a function pointer to print an element within the queue
 * 
 * Return:
 *   none.
 */
void dynarray_print(struct dynarray* da, void (*p) (void* a)){
  assert(da);
  assert(da->data);

  for (int i = 0; i < da->capacity; ++i)
  {
    printf("%d: ", i);
    if (da->data[i] == NULL){
      printf("NULL\n");
      continue;
    }
    p(da->data[i]);

  }

  return;

}
