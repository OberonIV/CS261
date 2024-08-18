/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Carl Jeffri Resurreccion
 * Email: resurrec@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  // Allocates memory for empty BST, returns bst.
  struct bst* bst = malloc(sizeof(struct bst));
  bst->root = NULL;
  return bst;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free_node(struct bst_node* node){
	// If bst node exists, recursively frees left and right subtrees, then node itself
	if(node){
		bst_free_node(node->left);
		bst_free_node(node->right);
		free(node);
	}
}

void bst_free(struct bst* bst) {
  // Calls bst_free_node to free all nodes, then frees bst
  bst_free_node(bst->root);
  free(bst);
  return;
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
 
int bst_count_node(struct bst_node* node){
	// If node doesnt exist, return 0
	if(!node){
		return 0;
	}
	// Recursively counts left and right subtrees, plus 1 for current node
	return 1 + bst_count_node(node->left) + bst_count_node(node->right);
}

int bst_size(struct bst* bst) {
  // Calls bst_count_node to return the size
  return bst_count_node(bst->root);
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
  // Allocates new node
  struct bst_node* node = malloc(sizeof(struct bst_node));
  // Initalizes key and value for the new node, and sets left and right leafs to NULL
  node->key = key;
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  // If the root of the bst does not exist, set the new node as root
  if(!bst->root){
	  bst->root = node;
	  return;
  }
  // Current is the current node, parent is the node right above current
  struct bst_node* current = bst->root;
  struct bst_node* parent = NULL;
  // Logic for traversing through the bst. Will traverse until it hits NULL
  while(current != NULL){
	  // Sets parent to current node. When current hits NULL, parent will be the node above it
	  parent = current;
	  // If key is less than the current key, go to left subtree
	  if(key < current->key){
		  current = current->left;
	  }
	  // If key is more than the current key, go to the right subtree
	  else{
		  current = current->right;
	  }
  }
  // If the key is less than the parent key, set the new node to the parents left subetree
  if(key < parent->key){
	  parent->left = node;
  }
  // If they key is more than the parents key, set the new node to the parents right subtree
  else{
	  parent->right = node;
  }
  return;
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */

// Helper function for removing a node with no children
void bst_remove_empty(struct bst* bst, struct bst_node* current, struct bst_node* parent){
	// If the node to be removed is the root, free the root
	if(current == bst->root){
		free(current);
		bst->root = NULL;
	}
	// If the node to be removed is the parents left node, set parent->left to null
	else if(parent->left == current){
		parent->left = NULL;
	}
	// If the node to be removed is the parents right node, set parent->right to null
	else{
		parent->right = NULL;
	}
	// Frees memory for current node
	free(current);	
}

// Helper function for removing a node with one child
void bst_remove_one(struct bst* bst, struct bst_node* current, struct bst_node* parent){
	// Determines whether the child of the node to be removed is on the left of the right
	struct bst_node* child;
	if(current->left != NULL){
		child = current->left;
	}
	else{
		child = current->right;
	}
	// IF the node to be removed is the root, free the root
	if(current == bst->root){
		free(current);
	}
	// Logic for replacing the node with its child node
	else if(parent->left == current){
		// If the node to be removed is the parents left child, replace the current node with the currents child
		parent->left = child;
	}
	else{
		// If the node to be removed is the parents right child, replace the current node with the currents child
		parent->right = child;
	}
	// Frees memory for current node
	free(current);	
}

// Helper function for removing a node with two children
void bst_remove_two(struct bst* bst, struct bst_node* current, struct bst_node* parent){
	// Initialize successor_parent, and successor
	struct bst_node *successor_parent = current;
	struct bst_node *successor = current->right;
	// Logic to find in-order successor, which is the leftmost node of the right subtree
	// Traverses to the bottom of the right subtree
	while(successor->left != NULL){
		// Sets the successor node to the leftmost node 
		successor_parent = successor;
		successor = successor->left;
	}
	// Sets the current node to the successor value found above
	current->key = successor->key;
	current->value = successor->value;
	// Logic for removing the successor node
	if(successor_parent->left == successor){
		successor_parent->left = successor->right;
	}
	else{
		successor_parent->right = successor->right;
	}
	// Frees memory for the successor
	free(successor);
 }
 
void bst_remove(struct bst* bst, int key) {
  // Sets current node and parent node
  struct bst_node* current = bst->root;
  struct bst_node* parent = NULL;
  // Logic to traverse down the tree to find the node to be remove
  // While loop that will iterate until current and current->key equals the desired key
  while(current && current->key != key){
	  // Sets parent to current
	  parent = current;
	  // If the key is less than the current key, move to left subtree
	  if(key < current->key){
		  current = current->left;
	  }
	  // If they key is more than the current key, move to the right subtree
	  else{
		  current = current->right;
	  }
  }
    // Logic for determining which remove function to call
	if(current->left == NULL && current->right == NULL){
		// If current->left or current->right is NULL, call remove_empty
		bst_remove_empty(bst, current, parent);
	}
	else if(current->left == NULL || current->right == NULL){
		// If only one of the subtrees is NULL, call remove_one
		bst_remove_one(bst, current, parent);
	}
	else{
		// If neither are NULL, call remove_two
		bst_remove_two(bst, current, parent);
	}
  return;
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  // Sets current node to the root
  struct bst_node* current = bst->root;
  // Logic for traversing down the tree
  while(current != NULL){
	  // If the key equals the key of the current node, return the value of the node
	  if(key == current->key){
		  return current->value;
	  }
	  // If they key is less than the current nodes key, move to the left subtree
	  else if(key < current->key){
		  current = current->left;
	  }
	  // If the key is more than the current nodes key, move to the right subtree
	  else{
		  current = current->right;
	  }
  }
  return NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/
/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
 // Helper function that recursively finds the height of each subtree
 int bst_node_height(struct bst_node* node){
	// Return nothing if the node is empty
	if(node == NULL){
		return -1;
	}
	// Recursively call bst_node_height to find the height of the left and the right subtree
	int left_height = bst_node_height(node->left);
	int right_height = bst_node_height(node->right);
	// Logic for determining what height to return
	if(left_height > right_height){
		// If the left subtree is higher than the right subtree, return the height plus one for the current node
		return 1 + left_height;
	}
	else{
		// If the right subtree is higher, or they are equal, return the height plus one for the current node
		return 1 + right_height;
	}
 }
 
 int bst_height(struct bst* bst) {
	 // Calls the helper function to find the height
	 return bst_node_height(bst->root);
 }

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
// Helper function for recursively calling bst_path_exists
int bst_path_exists(struct bst_node* node, int sum){
	// If node is NULL, return 0, meaning no path exists
	if(node == NULL){
		return 0;
	}
	// Calculate the sum we need by subtracting current key from the sum we want
	int remaining_sum = sum - node->key;
	// Logic for checking if we are at the bottom of the tree
	if(node->left == NULL && node->right == NULL){
		// If the remaining sum is zero, then the path exists
		if(remaining_sum == 0){
			return 1;
		}
		// If the sum is not 0, the path doesnt exist
		else{
			return 0;
		}
	}
	// Recursively calls path_exists to see if path exists in either left or right subtree
	int left_sum_exists = bst_path_exists(node->left, remaining_sum);
	int right_sum_exists = bst_path_exists(node->right, remaining_sum);
	// If left_sum_exists or right_sum_exists is true, then the path exists, return 1
	if(left_sum_exists != 0 || right_sum_exists != 0){
		return 1;
	}
	// If neither are true, return 0
	else{
		return 0;
	}
}

int bst_path_sum(struct bst* bst, int sum) {
  // Calls the helper function to determine whether the path exists
  return bst_path_exists(bst->root, sum);
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
// Helper function that recursively calls range_sum_node 
int bst_range_sum_node(struct bst_node* node, int lower, int upper){
	// If node is NULL, return 0, meaning no sum
	if(node == NULL){
		return 0;
	}
	// Initialize sum to 0
	int sum = 0;
	// If the current key is within the range, add it to the sum
	if(node->key >= lower && node->key <= upper){
		sum += node->key;
	}
	// If the node key is gerater than the lower value, go to the left subtree and call range_sum_node again
	if(node->key > lower){
		sum += bst_range_sum_node(node->left, lower, upper);
	}
	// If the node key is lower than the upper value, go to the right subtree and call range sum node again
	if(node->key < upper){
		sum += bst_range_sum_node(node->right, lower, upper);
	}
	// Returns the total sum
	return sum;
}
int bst_range_sum(struct bst* bst, int lower, int upper) {
  // Calls the helper function to returnt he total sum of keys within the range
  return bst_range_sum_node(bst->root, lower, upper);
}

/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack* stack;
};

/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */
struct bst_iterator* bst_iterator_create(struct bst* bst) {
  // Allocates the memory for the iteratior
  struct bst_iterator* iter = (struct bst_iterator*)malloc(sizeof(struct bst_iterator));
  // Calls stack_create from stack.c to initalize the stack in the iterator
  iter->stack = stack_create();
  // Sets the current node to the root
  struct bst_node* current = bst->root;
  // Logic for traversing down tree
  while(current != NULL){
	  // Pushes the current node to the stack
	  stack_push(iter->stack, current);
	  // Moves down the left subtree
	  current = current->left;
  }
  return iter;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator* iter) {
  // Uses stack_free from stack.c to free the iterators stack
  stack_free(iter->stack);
  // Frees the memory allocated for the iterator
  free(iter);
  return;
}

/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator* iter) {
  // Returns true if stack is not empty, using stack_isempty from stack.c
  return !stack_isempty(iter->stack);
}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator* iter, void** value) {
	// Pops node from the top of the stack
	struct bst_node* current = stack_pop(iter->stack);
	// If the stack is empty, set value to NULL and returns 0
	if(current == NULL){
		*value = NULL;
		return 0;
	}
	// Sets value to current value
	*value = current->value;
	// Sets right to the right child of current node
	struct bst_node* right = current->right;
	// Logic for traversing down tree
	while(right != NULL){
		// Pushes current->right to the stack
		stack_push(iter->stack, right);
		// Sets right to the left child of right
		right = right->left;
	}
	// Returns the current key
	return current->key;
}
