#ifndef MEM_WORK
#define MEM_WORK
#include "types.h"

void                stack_ctor(stack* values, int capasity);
void                stack_realloc(stack* values);
void                stack_dtor(stack* values);
void                pointer_to_data_canary(stack* values);

#endif