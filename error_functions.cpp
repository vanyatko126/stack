#include "error_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "mem_work.h"
#include "tools.h"

static const unsigned long long meow_1 = 52525252;
static const unsigned long long meow_2 = 25252525;

unsigned long long djb2_hash_struct(stack* values, size_t size) 
{
    char* bytes = (char*)(values);
    unsigned long long hash = meow_1;
    for (size_t i = 0; i < size; i++) 
    {
        hash = ((hash << 5) + hash) + bytes[i]; 
    }
    return hash;
}
unsigned long long djb2_hash_data(stack_value* values, size_t size)
{
    unsigned long long* bytes = (unsigned long long*)values;
    unsigned long long hash = meow_2;
    for (size_t i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + bytes[i]; 
    }
    return hash;
}

errors errors_check(stack* values)
{
    if(values->left_canary != CANARY_VALUE)
        return CANARY_STRUCT_ERROR;
    else if(values->right_canary != CANARY_VALUE)
        return CANARY_STRUCT_ERROR;

    values->data = (stack_value*)((char*)values->data - sizeof(canary_type));

    if(*(canary_type*)values->data != CANARY_VALUE)
        return CANARY_DATA_ERROR;
    
    values->data = (stack_value*)((char*)values->data + sizeof(canary_type) + values->capacity * sizeof(stack_value));

    if(*(canary_type*)values->data != CANARY_VALUE)
        return CANARY_DATA_ERROR;

    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value));
    
    if(values->size < 0)
        return DATA_ERROR;
    if(compare(values->hash_struct, djb2_hash_struct(values, SIZE_OF_MEM)) == 0)
        return HASH_STRUCT_ERROR;
    if(compare(values->hash_data, djb2_hash_data(values->data, values->size)) == 0)
        return HASH_DATA_ERROR;
    return NO_ERROR;
}
void exit_assert(stack* values, errors error)
{
    switch (error)
    {
    case CANARY_STRUCT_ERROR:
        fprintf(stderr, "Your struct is broken\n");
        break;
    case CANARY_DATA_ERROR:
        fprintf(stderr, "Your data is broken\n");
        break;
    case HASH_STRUCT_ERROR:
        fprintf(stderr, "Your hash of struct is incorrect\n");
        break;    
    case HASH_DATA_ERROR:
        fprintf(stderr, "Your hash of data is incorrect\n");
        break;
    case DATA_ERROR:
        fprintf(stderr, "You popped too much times\n");
        break;
    default:
        fprintf(stderr, "Undefined error\n");
        break;
    }
    stack_dtor(values);
    exit(EXIT_FAILURE);
}