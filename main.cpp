#include "stk_header.h"

int main()
{
    Stack stk = {};
    StackCtor (&stk, BASE_STK_SIZE);
    StackPush (&stk, 1);
    StackPush (&stk, 3); 
    StackDtor(&stk);
    return 0;
}
