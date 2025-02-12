#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "mem_work.h"
#include "tools.h"
#include "error_functions.h"

int main(int argc, char* argv[])
{
    char* name_file = check_args(argc, argv);
    stack_value deleted_number = 0;
    stack stk = {};
    stack_ctor(&stk, 10);
    push(&stk, 53);
    pop(&stk, &deleted_number);
    dump(&stk);
    stack_dtor(&stk);
    return 0;
}