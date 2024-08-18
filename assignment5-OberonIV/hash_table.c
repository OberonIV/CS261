/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Carl Jeffri Resurreccion
 * Email: resurrec@oregonstate.edu
 */

#include <stdlib.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"


// HT WILL USE CHAINING TO RESOLVE COLLISIONS


/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */

struct ht{
	// Holds the hash tables size and capacity, as well as an array of buckets
	struct ht_entry **buckets;
	int size;
	int capacity;
};

// This is the structure that represents entries in the hash table. It defines 
// each entries key and value, as well as a next pointer, in order to resolve
// collision via linked list
 struct ht_entry{
	 void *key;
	 void *value;
	 struct ht_entry *next;
 };
 

/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
	// Defines an initial capacity and allocates memory for the table
	int initial_capacity = 2;
	struct ht *table = malloc(sizeof(struct ht));
	// Initalizes capacity and size, and allocates memory for the buckets
	table->capacity = initial_capacity;
	table->size = 0;
	table->buckets = malloc(table->capacity * sizeof(struct ht_entry *));
	// Initalizes all buckets to NULL and returns the table
	for(int i = 0; i < table->capacity; i++){
		table->buckets[i] = NULL;
	}
	return table;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
	// Iterates through the entire table
	for(int i = 0; i < ht->capacity; i++){
		// Gets the front of the bucket
		struct ht_entry *entry = ht->buckets[i];
		// While in the bucket
		while(entry){
			// Gets next entry in *next, frees current then goes to next
			struct ht_entry *next = entry->next;
			free(entry);
			entry = next;
		}
	}
	// Frees the bucket array, then the hash table itself
	free(ht->buckets);
	free(ht);
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){
	// Returns true if the table is empty, false otherwise
	return ht->size == 0;
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 *
 * Params:
 *   ht - the hash table whose size is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return the size of the given hash table.
 */
int ht_size(struct ht* ht){
	// Returns the size of the table
	return ht->size;
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash function is passed in as a function pointer, stored in 'convert'
 * Assuming the passed in hash function is well designed.
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element used to calculate the index
 *   convert - a pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the index value of 'key' in the hash table .
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
	// Uses convert function pointer to get integer hashcode
	int hashcode = convert(key);
	// Gets the index
	int index = hashcode % ht->capacity;
	// Error handling in case it calculates a negative index
	if(index < 0){
		index += ht->capacity;
	}
	
	return index;
}

// This is a helper function that handles the resizing of the hash when it exceeds the load factor
void ht_resize(struct ht* ht, int(*convert)(void*)){
	// Keeps track of the old capacity, then doubles the capacity of the array
	int old_capacity = ht->capacity;
	ht->capacity *= 2;
	
	//Allocates memory for a new bucket array, and initalizes all buckets in the array to NULL
	struct ht_entry **new_buckets = malloc(ht->capacity * sizeof(struct ht_entry *));
	for(int i = 0; i < ht->capacity; i++){
		new_buckets[i] = NULL;
	}
	
	// Function that handles rehashing
	for(int i = 0; i < old_capacity; i++){
		// Gets the front of the bucket
		struct ht_entry *entry = ht->buckets[i];
		while(entry){
			// While in the bucket, store the next entry in next, computes the new index by calling the hash function,
			// sets the entry to the rehashed index in the new bucket, and moves to the next entry
			struct ht_entry *next = entry->next;
			int new_index = ht_hash_func(ht, entry->key, convert);
			entry->next = new_buckets[new_index];
			new_buckets[new_index] = entry;
			entry = next;
		}
	}
	
	//Frees the old bucket, and makes the hash table point to the new bucket array
	free(ht->buckets);
	ht->buckets = new_buckets;
}

// This is a helper function that compares two integer keys
int ht_key_compare(void* key1, void* key2){
	// Dereferences, casts to int and checks if the two ints are equal, returns true if they are, false otherwise
	return *((int*)key1) == *((int*)key2);
}

/*
 * This function should insert a given element into a hash table with a
 * specified key. Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){
	// Gets the current load factor, and if its greater than 4 (since we are using chaining), resizes the table
	float load_factor = (float)ht->size / ht->capacity;
	if(load_factor >= 4.0){
		ht_resize(ht, convert);
	}
	
	// Gets the index of the bucket the key should be inserted, and returns the front of that bucket
	int index = ht_hash_func(ht, key, convert);
	struct ht_entry *entry = ht->buckets[index];
	
	// While in the bucket
	while(entry){
		// If the key already exists, overwrite that key, and exit out
		if(ht_key_compare(entry->key, key)){
			entry->value = value;
			return;
		}
		entry = entry->next;
	}
	// If it doesnt exist, create a new entry, sets the entry key and value,
	// insert the entry at the front of the bucket, update bucket head, and increment the table size
	struct ht_entry *new_entry = malloc(sizeof(struct ht_entry));
	new_entry->key = key;
	new_entry->value = value;
	new_entry->next = ht->buckets[index];
	ht->buckets[index] = new_entry;
	
	ht->size++;
}


/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the value of the corresponding 'key' in the hash table .
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
	// Gets the index of the bucket the key should be inserted, and returns the front of that bucket
	int index = ht_hash_func(ht, key, convert);
	struct ht_entry *entry = ht->buckets[index];
	
	// While in the bucket
	while(entry){
		// Iterates through the bucket and returns the entry value if it matches the key
		if(ht_key_compare(entry->key, key)){
			return entry->value;
		}
		entry = entry->next;
	}
	// If it exists the bucket without finding the key, return NULL, meaning the key doesnt exist
	return NULL;
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){
	// Gets the index of the bucket the key should be inserted, and returns the front of that bucket,
	// and create a pointer that keeps track of the previous entry
	int index = ht_hash_func(ht, key, convert);
	struct ht_entry *entry = ht->buckets[index];
	struct ht_entry *prev = NULL;

	// While in the bucket
	while(entry){
		// If the key you need to remove is found
		if(ht_key_compare(entry->key, key)){
			if(prev){
				// Removes the element by replacing the element with the entry after the element
				prev->next = entry->next;
			}
			else{
				// Removes the element by replacing it with the entry after the element
				ht->buckets[index] = entry->next;
			}
			// Frees the entry, decrements the size of the table, and breaks out of the function
			free(entry);
			ht->size--;
			return;
		}
		// Sets prev to current, and entry to next, in order to iterate through the array
		prev = entry;
		entry = entry->next;
	}
} 