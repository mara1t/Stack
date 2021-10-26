
#include "stk_header.h"

int main()
{
    Stack stk = {};
    StackCtor (&stk, BASE_STK_SIZE);
    StackPush (&stk, 1);
    StackPush (&stk, 2); 
    stk.data[1] = 0;
    StackDtor(&stk);
    return 0;
}
