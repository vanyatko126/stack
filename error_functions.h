#ifndef ERROR_FUNCTIONS
#define ERROR_FUNCTIONS

#include "types.h"

errors              errors_check(stack* values);
void                exit_assert(stack* values, errors error);
unsigned long long  djb2_hash_struct(stack* values, size_t size);
unsigned long long  djb2_hash_data(stack_value* values, size_t size);
 
#endif