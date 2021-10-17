#include <stdio.h>
#include <stdlib.h>
#include "header_stk.h"
#define NDEBUG 1

int main()
{
    Stack stk = {};
    StackCtor (&stk, 10);
    StackPush (&stk, 10);
    StackPush (&stk, 20);
    double x = StackPop (&stk);
    double x1 = 0;
   // if (StackPop(&stk, &x1) == ERR_UNDERFLOW) 
    printf("%lg", StackPop(&stk));
    for(int i = 0; i < 100; i++)
    {
        StackPush(&stk, i);
    }
    StackDtor(&stk);
    return 0;
}

