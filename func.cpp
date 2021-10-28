#define TWO_LVL_PROTECT
#include "stk_header.h"

void StackPush (Stack* stk, const data_type value)
{
    CHECK_STK_;
   
    if (stk->size == stk->capacity)
        StackResize (stk, 2 * stk->capacity);

    *(stk->data + stk->size) = value;
    stk->size++;

    #ifdef TWO_LVL_PROTECT
    
        stk->hash = MurMurHash(stk);
    
    #endif

    CHECK_STK_;
}

int StackPop (Stack *stk)
{
    CHECK_STK_;

    if (stk->size <= 0) {
        exit(STACK_UNDERFLOW);
    }
    stk->size--;

    #ifdef TWO_LVL_PROTECT
        stk->hash = MurMurHash(stk);
    #endif

    CHECK_STK_;

    return stk->data[stk->size];
}

void StackCtor (Stack* stk, const int capacity)  
{
    assert(stk != NULL && capacity >= 0);

    stk->capacity = capacity;
    stk->size = 0;

    #ifdef NO_PROTECT
    
        stk->data = (data_type*) calloc(1, stk->capacity * sizeof(data_type));
    
    #endif
    
    #ifndef NO_PROTECT
    
        stk->data_canary = (char*) calloc(1, stk->capacity * sizeof(data_type) + 2 * sizeof(can_type));
        stk->data = (data_type*)(stk->data_canary + sizeof(can_type));

        stk->left_stk_can  = const_l_stk_canary;
        stk->right_stk_can = const_r_stk_canary;

        LEFT_DATA_CAN_ = const_l_stk_canary;
        RIGHT_DATA_CAN_ = const_r_stk_canary;
    
    #endif
    
    #ifdef TWO_LVL_PROTECT
    
       stk->hash = MurMurHash(stk);
    
    #endif
   
    CHECK_STK_;
}

void StackDtor (Stack* stk)
{
    CHECK_STK_;

    free(stk->data_canary);
    free(stk->data);
    stk->size = -1;
    stk->capacity = 0;
    stk->data = NULL;
}

void StackResize (Stack* stk, int new_capacity)
{
    CHECK_STK_;

    stk->capacity = new_capacity;
    
    #ifndef NO_PROTECT
    
        const int cons = stk->right_stk_can;
        RIGHT_DATA_CAN_  = 0;
        stk->data_canary = (char*) realloc (stk->data_canary, new_capacity*sizeof(data_type) + 2 * sizeof(can_type));
        stk->data = (data_type*)(stk->data + sizeof(can_type));
        RIGHT_DATA_CAN_ = cons;
    
    #endif
    
    #ifdef NO_PROTECT
    
        stk->data = (data_type*) realloc (stk->data, new_capacity*sizeof(data_type));
    
    #endif

    CHECK_STK_;
}

int StackPeek (Stack* stk)
{
    CHECK_STK_;
    return stk->data[stk->size];
}

void StackDump (Stack* stk, const int line, const int error)
{
    FILE* canary_file = fopen("canary_errors.txt", "w");
    fprintf(canary_file, "ERROR types:\n"
    "NO_ERROR              =  0\n"
    "STACK UNDERFLOW       = -1\n"
    "STACK OVERFLOW        = -2\n"
    "DATA SIZE_ERR         = -3\n"
    "DATA CAPACITY_ERR     = -4\n"
    "NULL STK_POINTER      = -5\n"
    "DATA LEFT_CANARY_ERR  = -6\n"
    "DATA RIGHT_CANARY_ERR = -7\n"
    "STK LEFT CANARY_ERR   = -8\n"
    "STK RIGHT CANARY ERR  = -9\n"
    "HASH ERR              = -10\n\n");

    #ifdef NO_PROTECT
        fprintf(canary_file, "<< No canary protect >>\n");
    #else 
        #ifdef TWO_LVL_PROTECT
            fprintf(canary_file, "<< Canary protection and hash protection >>\n");
        #else
            fprintf(canary_file, "<< Canary protection >>\n");
        #endif
        if (error < 0)
        {
            fprintf(canary_file, "Stack has error type: %d on line %d\n", error, line);
            fprintf(canary_file, "*stk left canary   = %d\n"
                                 " stk right canary  = %d\n"
                                 "*data left canary  = %d\n"
                                 " data right canary = %d\n"
                                 "*default left canary  = %d\n"
                                 " default right canary = %d\n",
            stk->left_stk_can, stk->right_stk_can, LEFT_DATA_CAN_, RIGHT_DATA_CAN_, const_l_stk_canary, const_r_stk_canary);
        }
    #endif

    fprintf(canary_file, "*data pointer = %p; *stack size = %d; *stack capacity = %d\n", stk->data, stk->size, stk->capacity);
    
    for (int counter = 0; counter < stk->size; counter++)
    {
        fprintf(canary_file, "data[%d] = %d\n", counter, stk->data[counter]);
    }
    fclose(canary_file);
}

int MurMurHash (Stack* stk)
{
    int sum = 0;
    for (int counterj = 0; counterj < stk->size; counterj++)
    {
        for (int counteri = counterj; counteri < stk->size; counteri++)
        {
            sum += stk->data[counteri] * stk->data[counterj];
        }
    }
    return sum;
}

int check_stk_err(Stack* stk, const int line)
{
    const int error = stk_status(stk);
    if (error < 0)
    {
        StackDump(stk, line, error);
    }
    return error;
}

int stk_status(Stack* stk)
{
    #ifndef NO_PROTECT

        if (LEFT_DATA_CAN_ != const_l_stk_canary)   
            return DATA_LEFT_CANARY_ERR;

        else if (RIGHT_DATA_CAN_ != const_r_stk_canary)  
            return DATA_RIGHT_CANARY_ERR;

        else if (stk->left_stk_can != const_l_stk_canary)
            return STK_LEFT_CANARY_ERR;

        else if (stk->right_stk_can != const_r_stk_canary)   
            return STK_RIGHT_CANARY_ERR;
    
    #endif
    
    #ifdef TWO_LVL_PROTECT
    
        if ( MurMurHash(stk) != stk->hash)
            return HASH_ERR;
    
    #endif

    if (stk->size <= -1)
        return DATA_SIZE_ERR;

    else if (stk == NULL)  
        return NULL_STK_POINTER;

    else if (stk->capacity <= 0)  
        return DATA_CAPACITY_ERR;

    return NO_ERROR;
}
