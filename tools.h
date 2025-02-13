#ifndef TOOLS
#define TOOLS
#include "types.h"

void                push(stack* values, stack_value push_number);
void                pop(stack* values, stack_value* deleted_number);
void                dump(stack* values);
int                 compare(long long int num1, long long int num2);

#endif
//TOOLS