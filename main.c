#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX_SIZE 20
#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101

typedef struct Stack_tag {
    int data[STACK_MAX_SIZE];
    int size;
} Stack_t;

void push (Stack_t *stack, const int value);
void print_stack (Stack_t *stack);
int pop (Stack_t *stack);

void main()
{
    Stack_t stack;
    *stack.data = (int* ) malloc(STACK_MAX_SIZE* sizeof(int));

    stack.size = 0;

    push(&stack, 3);
    push(&stack, 2);
    push(&stack, 1);
    printf("%d", pop(&stack));
    pop(&stack);
}

void push (Stack_t *stack, const int value) {
    if (stack->size >= STACK_MAX_SIZE) {
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
}

int pop (Stack_t *stack) {
    if (stack->size == 0) {
        exit(STACK_UNDERFLOW);
    }
    stack->size--;
    return stack->data[stack->size];
}

void print_stack (Stack_t *stack) {
    if (stack->size == 0) {
        exit(STACK_UNDERFLOW);
    }
    while (stack->size > 0)
    {
        printf("%d", stack->data[stack->size]);
        stack->size--;
    }
}
