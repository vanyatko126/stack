#ifndef ERROR_FUNCTIONS
#define ERROR_FUNCTIONS

#include "types.h"

errors              errors_check(stack* values);
void                exit_assert(stack* values, errors error);
unsigned long long  djb2_hash(stack* values, size_t size);
 
#endif