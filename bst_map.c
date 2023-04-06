#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef struct bst_map_node {
	struct bst_map_node* left;
	struct bst_map_node* right;
	void* key;
	void* value;
} bst_map_node_t;

//  0: lhs == rhs
//  > 0: lhs > rhs
// < 0: lhs < rhs
typedef int (*comparator_t)(void* lhs, void* rhs);
typedef struct {
	bst_map_node_t* root;
	comparator_t comparator;
} bst_map_t;

void bst_map_init(bst_map_t* bst_map, comparator_t comparator) {
	bst_map->root = NULL;
	bst_map->comparator = comparator;
}

bst_map_node_t** bst_map_insert2(bst_map_node_t** comparand_ptr,
								 void* key,
								 comparator_t comparator) {
	bst_map_node_t* comparand = *comparand_ptr;

	if (!comparand)
		return comparand_ptr;

	int result = comparator(key, comparand->key);
	// data > comparand
	if (result < 0) {
		return bst_map_insert2(&comparand->left, key, comparator);
	}
	// data < comparand
	else if (result > 0) {
		return bst_map_insert2(&comparand->right, key, comparator);
	}
	// data == comparand
	else
		return comparand_ptr;
}

void bst_map_insert(bst_map_t* bst_map, void* key, void* value) {
	bst_map_node_t* node = malloc(sizeof(bst_map_node_t));
	node->left = NULL;
	node->right = NULL;
	node->key = key;
	node->value = value;

	if (!bst_map->root) {
		bst_map->root = node;
	} else {
		bst_map_node_t** slot = bst_map_insert2(&bst_map->root, node->key, bst_map->comparator);
		*slot = node;
	}
}

// breadth-first traversal
void bst_map_print(bst_map_t* bst_map) {
	queue_t nodes;
	queue_init(&nodes);
	queue_push(&nodes, bst_map->root);

	bst_map_node_t* node;
	while ((node = queue_pop(&nodes)) != NULL) {
		printf("%s : %s\n", (char*)node->key, (char*)node->value);

		if (node->left)
			queue_push(&nodes, node->left);
		if (node->right)
			queue_push(&nodes, node->right);
	}
}

// breadth-first search
void* bst_map_get(bst_map_t* bst_map, void* key) {
	queue_t nodes;
	queue_init(&nodes);
	queue_push(&nodes, bst_map->root);

	bst_map_node_t* node;
	while ((node = queue_pop(&nodes)) != NULL) {
		if (strlen(key) == strlen(node->key) && strcmp(key, node->key) == 0)
			return node->value;
		if (node->left)
			queue_push(&nodes, node->left);
		if (node->right)
			queue_push(&nodes, node->right);
	}

	return NULL;
}

int compare_int(void* s1, void* s2) {
	int i1 = atoi(s1);
	int i2 = atoi(s2);

	return i1 - i2;
}

int main() {
	bst_map_t bst_map;
	bst_map_init(&bst_map, (comparator_t)compare_int);
	bst_map_insert(&bst_map, "8", "a");
	bst_map_insert(&bst_map, "3", "b");
	bst_map_insert(&bst_map, "10", "c");
	bst_map_insert(&bst_map, "1", "d");
	bst_map_insert(&bst_map, "6", "e");
	bst_map_insert(&bst_map, "14", "f");
	bst_map_insert(&bst_map, "4", "h");
	bst_map_insert(&bst_map, "7", "i");
	bst_map_insert(&bst_map, "13", "i");
	bst_map_print(&bst_map);

	char* c = bst_map_get(&bst_map, "10");
	printf("\nGET:\n10 : %s\n", c);
}