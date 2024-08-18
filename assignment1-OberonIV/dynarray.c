/*
 * This file is where you should implement your dynamic array.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Carl Jeffri Resurreccion
 * Email: resurrec@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include "dynarray.h"


/*
 * This is the definition of the dynamic array structure you'll use for your
 * implementation.  Importantly, your dynamic array implementation will store
 * each data element as a void* value.  This will permit data of any type to
 * be stored in your array.  Because each individual element will be stored in
 * your array as type void*, the data array needs to be an array of void*.
 * Hence it is of type void**.
 *
 * You should not modify this structure.
 */
struct dynarray
{
    void** data;
    int size;
    int capacity;
};

/*
 * This function should allocate and initialize a new, empty dynamic array and
 * return a pointer to it.  The array you allocate should have an initial
 * capacity of 2.
 */
struct dynarray* dynarray_create()
{
    /*
     * FIX ME: 
     */
	 // Allocates memory for the array 'da' 
	struct dynarray* da = malloc(sizeof(struct dynarray));
	// Initilizes data, size and capacity members of the struct.
	da->data = malloc(2 * sizeof(void*));
	da->size = 0;
	da->capacity = 2;
    return da;
}

/*
 * This function should free the memory associated with a dynamic array.  In
 * particular, while this function should free up all memory used in the array
 * itself (i.e. the underlying `data` array), it should not free any memory
 * allocated to the pointer values stored in the array.  In other words, this
 * function does not need to *traverse* the array and free the individual
 * elements.  This is the responsibility of the caller.
 *
 * Params:
 * da - the dynamic array to be destroyed.  May not be NULL.
 */
void dynarray_free(struct dynarray* da)
{
    /*
     * FIX ME: 
     */
	 // Frees the memory allocated by da->data, then frees the array
	 free(da->data);
	 free(da);
    return;
}

/*
 * This function should return the size of a given dynamic array (i.e. the
 * number of elements stored in it, not the capacity).
 */
int dynarray_size(struct dynarray* da)
{
    /*
     * FIX ME: 
     */
	 // Returns if array is empty. Thank you to Dominick Dorantes in the Discord for the idea.
	 if(da == NULL){
		 return 0;
	 }
	 // Returns the size member of the struct
    return da->size;
}

/*
 * This function should insert a new value to a given dynamic array.  For
 * simplicity, this function should only insert elements at the *end* of the
 * array.  In other words, it should always insert the new element immediately
 * after the current last element of the array.  If there is not enough space
 * in the dynamic array to store the element being inserted, this function
 * should double the size of the array.
 *
 * Params:
 * da - the dynamic array into which to insert an element.  May not be NULL.
 * val - the value to be inserted.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_insert(struct dynarray* da, void* val)
{
    /*
     * FIX ME: 
     */
	 if (da->size >= da->capacity){ // Checks if the size of the array is bigger than the capacity
		 da->capacity *= 2; // Doubles the capacity if size is too big
		 void** temp = realloc(da->data, da->capacity * sizeof(void*)); // Creates temp array
		 da->data = temp; // Copies the contents of the original array into the temporary array
	 }
	 da->data[da->size] = val; // Inserts the element at the end of array, given by da->data[da->size]
	 da->size++; // Increases the size of the array by one due to the new element
    return;
}

/*
 * This function should remove an element at a specified index from a dynamic
 * array.  All existing elements following the specified index should be moved
 * forward to fill in the gap left by the removed element.  In other words, if
 * the element at index i is removed, then the element at index i+1 should be
 * moved forward to index i, the element at index i+2 should be moved forward
 * to index i+1, the element at index i+3 should be moved forward to index i+2,
 * and so forth.
 *
 * Params:
 * da - the dynamic array from which to remove an element.  May not be NULL.
 * idx - the index of the element to be removed.  The value of `idx` must be
 *     between 0 (inclusive) and n (exclusive), where n is the number of
 *     elements stored in the array.
 */
void dynarray_remove(struct dynarray* da, int idx)
{
    /*
     * FIX ME: 
     */
	 // Shifts all array elements to the left, removing the element at 'idx'. 
	 for(int i = idx; i < da->size - 1; i++){
		 da->data[i] = da->data[i + 1]; // The element in data[i] will be replaced by the element in [i+1], removing what was originally in [i]. Then, all elements after will be shifted to the left so there is no empty space
	 }
	 da->size--; // Decreases the size of the array by one due to one less element. I believe the element at the end will be null (?)
    return;

}

/*
 * This function should return the value of an existing element a dynamic
 * array. Note that this value should be returned as type void*.
 *
 * Params:
 * da - the dynamic array from which to get a value.  May not be NULL.
 * idx - the index of the element whose value should be returned.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 */
void* dynarray_get(struct dynarray* da, int idx)
{
    /*
     * FIX ME: 
     */
	 // Returns whatever the element is in data[idx]
    return da->data[idx];
}

/*
 * This function should update (i.e. overwrite) the value of an existing
 * element in a dynamic array.
 *
 * Params:
 *   da - the dynamic array in which to set a value.  May not be NULL.
 *   idx - the index of the element whose value should be updated.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 *   val - the new value to be set.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_set(struct dynarray* da, int idx, void* val)
{
    /*
     * FIX ME: 
     */ 
	 //Sets the array index to whatever 'val' is
	da->data[idx] = val;
	return; 
}
