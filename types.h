#ifndef TYPES
#define TYPES

#include <stdio.h>
#include <math.h>

typedef double              stack_value;
typedef long long int       canary_type;
typedef unsigned long long  hash_type;

typedef struct 
{
    canary_type left_canary = 0;
    stack_value* data = NULL;
    int capacity = 0;
    int size = 0;
    canary_type right_canary = 0;

    hash_type hash_struct = 0;
    hash_type hash_data = 0;
} stack;

static const int               POIZON = NAN;
static const long long int     CANARY_VALUE = 0xAB0BA;
static const int               COUNT_OF_CANARY = 2;
static const int               SIZE_OF_MEM = sizeof(stack) - 2 * sizeof(hash_type);

enum errors
{
    CANARY_STRUCT_ERROR = 0,
    CANARY_DATA_ERROR   = 1,
    HASH_STRUCT_ERROR   = -1,
    HASH_DATA_ERROR     = 3,
    DATA_ERROR          = 2,
    NO_ERROR            = 52,
};

#endif
//TYPES