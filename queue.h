#include <stdlib.h>

typedef struct queue_node {
	void* data;
	struct queue_node* next;
} queue_node_t;

typedef struct {
	queue_node_t* head;
} queue_t;

void queue_init(queue_t* queue) {
	queue->head = NULL;
}

queue_node_t* queue_push(queue_t* queue, void* data) {
	queue_node_t* node = malloc(sizeof(queue_node_t));
	node->data = data;
	node->next = NULL;

	if (!queue->head) {
		return queue->head = node;
	}

	queue_node_t* temp = queue->head;
	while (temp->next) {
		temp = temp->next;
	}
	temp->next = node;
	return temp->next;
}

void* queue_pop(queue_t* queue) {
	if (queue->head == NULL)
		return NULL;

	void* ret = queue->head->data;

	queue_node_t* temp = queue->head;
	queue->head = queue->head->next;
	free(temp);

	return ret;
}