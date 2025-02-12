#include "tools.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "mem_work.h"
#include "error_functions.h"

void push(stack* values, stack_value push_number)
{
    values->data[values->size] = push_number;
    values->size++;
    stack_realloc(values);
    values->hash_struct = djb2_hash(values, values->size);  

    errors err = errors_check(values);
    if(err != NO_ERROR)
        exit_assert(values, err);
}

void pop(stack* values, stack_value* deleted_number)    
{
    *deleted_number = values->data[values->size];
    values->data[values->size] = POIZON; 
    values->size--;
    stack_realloc(values);
    values->hash_struct = djb2_hash(values, values->size);

    errors err = errors_check(values);
    if(err != NO_ERROR)
        exit_assert(values, err);
    return;
}

void dump(stack* values)
{
    for(int i = 0; i < values->size; i++)
        printf("%lf ", values->data[i]);
    putchar('\n');
    printf("Left struct_canary  = %lld\n", values->left_canary);
    printf("Right struct_canary = %lld\n", values->right_canary);
    printf("Left data_canary = %lld\n", *(canary_type*)((char*)values->data - sizeof(canary_type)));
    printf("Right data_canary = %lld\n", *(canary_type*)((char*)values->data + values->capacity * sizeof(stack_value)));
    printf("Size of stack = %d\n", values->size);
    printf("Capacity of stack = %d\n", values->capacity);
    return;
}


int compare(long long int num1, long long int num2)
{
    return fabs(num1 - num2) <= 0.0001;
}

char* check_args(int argc, char* argv[])
{
    if(argc != 2) {
        printf(".\\stack.exe <name_file>\n");
        exit(1);
    }

    return argv[1];
}