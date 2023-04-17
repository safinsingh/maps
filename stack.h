#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_STACK_LEN 16

typedef struct {
	void** items;
	int len;
	int cap;
} stack_t;

void stack_init(stack_t* stack) {
	stack->items = (void**)malloc(DEFAULT_STACK_LEN * sizeof(void*));
	stack->len = 0;
	stack->cap = DEFAULT_STACK_LEN;
}

void stack_resize(stack_t* stack) {
	stack->cap *= 2;
	stack->items = (void**)realloc(stack->items, stack->cap * sizeof(void*));
}

void* stack_push(stack_t* stack, void* data) {
	if (stack->len == stack->cap)
		stack_resize(stack);
	stack->items[stack->len] = data;
	return stack->items[stack->len++];
}

void* stack_pop(stack_t* stack) {
	if (stack->len == 0)
		return NULL;
	return stack->items[--stack->len];
}
