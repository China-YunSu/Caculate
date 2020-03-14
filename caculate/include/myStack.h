#ifndef _MY_STACK_H_
#define _MY_STACK_H_

#include "boolean.h"

typedef struct MY_STACK {
	void **stack;
	int capacity;
	int top;
}MY_STACK; 

boolean initStack(MY_STACK **stack, int capacity);
void destoryStack(MY_STACK **stack);
boolean isStackEmpty(const MY_STACK *stack);
boolean isStackFull(const MY_STACK *stack);
boolean push(MY_STACK *stack, void *data);
void *pop(MY_STACK *stack);
void *readTop(const MY_STACK *stack);

#endif
