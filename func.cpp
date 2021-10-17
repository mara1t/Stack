#include "header_stk.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void StakPush (Stack* stk, double value)
{
    assert(stk != NULL);
    if (stk->size >= stk->capacity)
        StackResize (stk, 2*stk->capacity);

    stk->data[stk->size++] = value;
}

double StackPop (Stack *stk) 
{
    assert(stk->size >= 1);
    if (stk->size == 0) {
        exit(STACK_UNDERFLOW);
    }
    stk->size--;
    double val = stk->data[stk->size+1];
    StackResize(stk, stk->capacity - 1);
    return val;
}

void StackCtor(Stack* stk, int capacity)  
{
    assert(stk != NULL);
    stk->data = (double*) calloc(capacity, sizeof(double));
    stk->capacity = capacity;
    stk->size = 0;
}

void StackDtor(Stack* stk)
{
    assert(stk != NULL);
    free(stk->data);
    stk->data = NULL;
}
void StackResize(Stack* stk, int new_capacity)
{
    stk->data = (double*) realloc (stk->data, new_capacity*sizeof(double));
}
double Stack_peek(Stack* stk)
{
    return stk->data[stk->size];
}