#include "stack.h"

int main() {
	stack_t stack;
	stack_init(&stack);
	stack_push(&stack, (void*)1);
	stack_push(&stack, (void*)2);
	printf("%p\n", stack_pop(&stack));
	printf("%p\n", stack_pop(&stack));
}