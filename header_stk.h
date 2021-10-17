#define STACK_MAX_SIZE 20
#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
typedef struct Stack_tag{
    double* data;
    int size;
    int capacity; 
} Stack;

void StackPush (Stack *Mystack, const int value);
double StackPop (Stack *Mystack);
void StackCtor (Stack* stk, int capacity );
void StackDtor (Stack* stk);
void StackResize(Stack* stk, int new_capacity);