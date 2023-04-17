#include <stdlib.h>

typedef struct queue_node {
	void* data;
	struct queue_node* next;
} queue_node_t;

typedef struct {
	queue_node_t* head;
} queue_t;

void queue_init(queue_t* queue);
queue_node_t* queue_push(queue_t* queue, void* data);
void* queue_pop(queue_t* queue);