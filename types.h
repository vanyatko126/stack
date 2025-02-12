#ifndef TYPES
#define TYPES

#include <stdio.h>
#include <math.h>

static const int            POIZON = NAN;
static const long long int  CANARY_VALUE = 0xAB0BA;
static const int            COUNT_OF_CANARY = 2;

typedef double              stack_value;
typedef long long int       canary_type;
typedef unsigned long long  hash_type;

typedef struct {
    
    int capacity;
    int size;
    canary_type left_canary;
    canary_type right_canary;
    stack_value* data;
    hash_type hash_stack;
    hash_type hash_struct;
} stack;

enum errors
{
    CANARY_STRUCT_ERROR = 0,
    CANARY_DATA_ERROR   = 1,
    HASH_STRUCT_ERROR   = -1,
    DATA_ERROR          = 2,
    NO_ERROR            = 52,
};

#endif