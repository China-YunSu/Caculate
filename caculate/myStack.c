#include <stdio.h>
#include <malloc.h>

#include "./include/boolean.h"
#include "./include/MyStack.h"

boolean initStack(MY_STACK **stack, int capacity) {
	MY_STACK *res;

	if (NULL == stack || NULL != *stack || capacity <= 0) {
		return FALSE;
	}

	res = (MY_STACK *) calloc(sizeof(MY_STACK), 1);
	res->stack = (void **) calloc(sizeof(void *), capacity);
	res->capacity = capacity;
	res->top = 0;

	*stack = res;

	return TRUE;
}

void destoryStack(MY_STACK **stack) {
	if (NULL == stack || NULL == *stack) {
		return;
	}

	free((*stack)->stack);
	free(*stack);

	*stack = NULL;
}

boolean isStackEmpty(const MY_STACK *stack) {
	return NULL != stack && stack->top <= 0;
}

boolean isStackFull(const MY_STACK *stack) {
	return NULL != stack && stack->top >= stack->capacity;
}

boolean push(MY_STACK *stack, void *data) {
	if (NULL == stack || isStackFull(stack)) {
		return FALSE;
	}
	stack->stack[stack->top++] = data;

	return TRUE;
}

void *pop(MY_STACK *stack) {
	if (NULL == stack || isStackEmpty(stack)) {
		return NULL;
	}
	
	return stack->stack[--stack->top];
}

void *readTop(const MY_STACK *stack) {
	if (NULL == stack || isStackEmpty(stack)) {
		return NULL;
	}

	return stack->stack[stack->top - 1];
}
