#include "mem_work.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "types.h"

static const int divide_of_four = 4;
static const int double_change = 2;

void pointer_to_data_canary(stack* values)
{
    *(canary_type*)values->data = CANARY_VALUE;
    values->data = (stack_value*)((char*)values->data + sizeof(canary_type));

    values->data = (stack_value*)((char*)values->data + values->capacity * sizeof(stack_value));
    *(canary_type*)values->data = CANARY_VALUE;
    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value));
}

void stack_ctor(stack* values, int capasity)  
{
    values->left_canary  = CANARY_VALUE;
    values->right_canary = CANARY_VALUE;

    values->capacity     = capasity;
    values->size         = 0;
    values->data = (stack_value*) calloc(1, values->capacity * sizeof(stack_value) +
                                            COUNT_OF_CANARY  * sizeof(canary_type)); 
    assert(values->data != NULL);
    pointer_to_data_canary(values);
}

void stack_realloc(stack* values)   
{   
    if(values->size > values->capacity / divide_of_four && values->size < values->capacity)
        return;

    values->data = (stack_value*)((char*)values->data + values->capacity * sizeof(stack_value));
    *(canary_type*)values->data = POIZON;
    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value) - sizeof(canary_type));

    if (values->size >= values->capacity)
        values->capacity *= double_change;

    if(values->size <= values->capacity / divide_of_four)
        values->capacity /= double_change;
        
    stack_value* data_check = (stack_value*)realloc(values->data, values->capacity * sizeof(stack_value)
                                                                   + COUNT_OF_CANARY * sizeof(canary_type));
    assert(data_check != NULL);
    values->data = data_check;
    values->data = (stack_value*)((char*)values->data + values->capacity * sizeof(stack_value) + sizeof(canary_type));
    *(canary_type*)values->data = CANARY_VALUE;
    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value));
}
void stack_dtor(stack* values)
{
    values->data = NULL;
    values->size = 0;
    values->capacity     = 0;
    values->hash_struct  = 0;
    values->hash_stack   = 0;//memset!
    values->left_canary  = 0;
    values->right_canary = 0;
}
