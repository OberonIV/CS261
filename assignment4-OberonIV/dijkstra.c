/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Carl Jeffri Resurreccion
 * Email: resurrec@oregonstate.edu
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq.h"
#define DATA_FILE "airports.dat"
#define START_NODE 0

// Struct that represents each edge(path) in the graph
struct Edge{
	int path;
	int weight;
};
// Struct that represents each node(airport) in the graph
struct Node{
	int num_edge;
	struct Edge* edge;
};
// Function that frees the memory of the graph
void dijkstra_free(struct Node* graph, int n_nodes, int* edge_count){
	// Frees the memory for each edge in the graph
	for(int i = 0; i < n_nodes; i++){
		free(graph[i].edge);
	}
	// Frees the graph itself
	free(graph);
	// Frees the edge count
	free(edge_count);
}

void initalize_graph(int n_nodes, int** cost, int** prev, int** node){
	// Allocates memory for cost, prev, and node
	*cost = (int*)malloc(n_nodes * sizeof(int));
	*prev = (int*)malloc(n_nodes * sizeof(int));
	*node = (int*)malloc(n_nodes * sizeof(int));
	// Initalizes each value
	for(int i = 0; i < n_nodes; i++){
		// NOTE: Originally, I just initalized the cost to a randomly large number,
		// but the pseudocode in the assignment initalizes to infinity, so I included
		// limits.h to use INT_MAX
		(*cost)[i] = INT_MAX;
		(*prev)[i] = -1;
		(*node)[i] = 0;
	}
	
}
// Pseudocode from lecture 18
void dijkstra(struct Node* graph, int n_nodes, int start_node){
	// Initalizes variables
	int* cost;
	int* prev;
	int* node;
	// Initalizes priority queue
	struct pq* pq = pq_create(n_nodes);
	// Calls initalize_graph and sets the cost and prev of the start node to zero
	initalize_graph(n_nodes, &cost, &prev, &node);
	cost[start_node] = 0;
	prev[start_node] = start_node;
	// Inserts start node
	pq_insert(pq, (void*)(size_t)start_node, 0);
	while(!pq_isempty(pq)){
		for(int i = 0; i < n_nodes; i++){
			// Find the smallest node (lowest priority)
			int curr = (int)(size_t)pq_remove_first(pq);
			// Breaks if the cost of the smallest node is infinity
			if(cost[curr] == INT_MAX){
				break;
			}
			// Sets node to visited
			node[curr] = 1;
			// Updates the cost of each node
			for(int j = 0; j < graph[curr].num_edge; j++){
				// Sets destination node
				int dest = graph[curr].edge[j].path;
				// Sets edge weight 
				int weight = graph[curr].edge[j].weight;
				// Checks if theres a shorter path to dest
				if(cost[curr] + weight < cost[dest]){
					// Updates the cost and prev
					cost[dest] = cost[curr] + weight;
					prev[dest] = curr;
					// If the node isnt visited, insert to pq with new cost
					if(node[dest] == 0){
						pq_insert(pq,(void*)(size_t)dest, cost[dest]);
						// Marks node as visited
						node[dest] = 1;
					}
					else{
						// Updates pq with new dest priority
						pq_insert(pq,(void*)(size_t)dest, cost[dest]);
					}
				}
			}
		}
	}
	// Prints shortest path from start to node
	for(int i = 0; i < n_nodes; i++){
		printf("Cost to node %d: %d -- Previous node %d\n", i, cost[i], prev[i]);
	}
	// Frees Memory
	pq_free(pq);
	free(cost);
	free(prev);
	free(node);
}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);
	
	// Allocates memory for graph
	struct Node* graph = (struct Node*)malloc(n_nodes * sizeof(struct Node));
	// Allocates memory for edge count
	int* edge_count = (int*)malloc(n_nodes * sizeof(int));
	
	// Initalize edge array and edge count
	for(int i = 0; i < n_nodes; i++){
		graph[i].num_edge = 0;
		graph[i].edge = (struct Edge*)malloc(n_edges * sizeof(struct Edge)); 
		edge_count[i] = 0;
	}

	// Reads edges from file and populates graph array
	for(int i = 0; i < n_edges; i++){
		int curr, dest, weight;
		fscanf(file, "%d %d %d", &curr, &dest, &weight);
		int index = edge_count[curr]++;
		graph[curr].edge[index].path = dest;
		graph[curr].edge[index].weight = weight;
		graph[curr].num_edge++;
	}
	fclose(file);
	// Calls algorithm and frees memory when algorithm is done
	dijkstra(graph, n_nodes, START_NODE);
	dijkstra_free(graph, n_nodes, edge_count);
	return 0;
}
