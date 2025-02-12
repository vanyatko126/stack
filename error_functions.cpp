#include "error_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "mem_work.h"
#include "tools.h"

static const unsigned long long meow_1 = 52525252;
static const unsigned long long meow_2 = 25252525;

unsigned long long djb2_hash(stack* values, size_t size) 
{
    unsigned long long* bytes = (unsigned long long*) values;
    unsigned long long hash = 5381;
    size = sizeof(stack) - 2 * sizeof(hash_type);
    for (size_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + bytes[i]; // hash * 33 + bytes[i]
    }
    return hash;
}
unsigned long long djb2_hash(stack* values, size_t size) 
{
    unsigned long long* bytes = (unsigned long long*) values;
    unsigned long long hash = 5381;
    size = sizeof(stack) - 2 * sizeof(hash_type);
    for (size_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + bytes[i]; // hash * 33 + bytes[i]
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

    if(fabs (values->hash_struct - djb2_hash(values, values->size)) < 0.0000000000001)
        return HASH_STRUCT_ERROR;
    return NO_ERROR;
}
void exit_assert(stack* values, errors error)
{
    switch (error)
    {
    case CANARY_STRUCT_ERROR:
        printf("Your struct is broken\n");
        break;
    case CANARY_DATA_ERROR:
        printf("Your data is broken\n");
        break;
    case HASH_STRUCT_ERROR:
        printf("Your hash is incorrect\n");
        break;    
    case DATA_ERROR:
        printf("You popped too much times\n");
        break;
    default:
        printf("Undefined error\n");
        break;
    }
    stack_dtor(values);
    exit(1);
}
