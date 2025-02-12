#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef enum{
    CANARY_STRUCT_ERROR = 0,
    CANARY_DATA_ERROR = 1,
    HASH_STRUCT_ERROR = -1,
    DATA_ERROR = 2,
    NO_ERROR = 52
}errors;

static const int            POIZON = NAN;
static const long long int  CANARY_VALUE = 0xAB0BA;
static const int            COUNT_OF_CANARY = 2;

typedef double              stack_value;
typedef long long int       canary_type;
typedef unsigned long long  hash_type;


typedef struct{
    canary_type left_canary;
    stack_value* data;
    int capacity;
    int size;
    canary_type right_canary;

    hash_type hash_stack;
    hash_type hash_struct;
}stack;

void                stack_ctor(stack* values, int capasity);
void                push(stack* values, stack_value push_number);
void                pop(stack* values, stack_value* deleted_number);
void                stack_realloc(stack* values);
void                stack_dtor(stack* values);
void                dump(stack* values);
void                pointer_to_data_canary(stack* values);
void                exit_assert(stack* values, errors error);
char*               check_args(int argc, char* argv[]);
unsigned long long  djb2_hash(stack* values, size_t size); 
errors              errors_check(stack* values);


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

void pointer_to_data_canary(stack* values)
{
    *(canary_type*)values->data = CANARY_VALUE;
    values->data = (stack_value*)((char*)values->data + sizeof(canary_type));

    values->data = (stack_value*)((char*)values->data + values->capacity * sizeof(stack_value));
    *(canary_type*)values->data = CANARY_VALUE;
    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value));
}

void stack_ctor(stack* values, int capasity)  
{
    values->left_canary  = CANARY_VALUE;
    values->right_canary = CANARY_VALUE;

    values->capacity     = capasity;
    values->size         = 0;
    values->data = (stack_value*) calloc(1, values->capacity * sizeof(stack_value) +
                                            COUNT_OF_CANARY  * sizeof(canary_type)); 
    assert(values->data != NULL);
    pointer_to_data_canary(values);
    return;
}

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

void stack_realloc(stack* values)   
{   
    if(values->size > values->capacity / 4 && values->size < values->capacity)
        return;

    values->data = (stack_value*)((char*)values->data + values->capacity * sizeof(stack_value));
    *(canary_type*)values->data = POIZON;
    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value) - sizeof(canary_type));

    if (values->size >= values->capacity)
        values->capacity *= 2;

    if(values->size <= values->capacity / 4)
        values->capacity /= 2;
        
    stack_value* data_check = (stack_value*)realloc(values->data, values->capacity * sizeof(stack_value)
                                                                   + COUNT_OF_CANARY * sizeof(canary_type));
    assert(data_check != NULL);
    values->data = data_check;
    values->data = (stack_value*)((char*)values->data + values->capacity * sizeof(stack_value) + sizeof(canary_type));
    *(canary_type*)values->data = CANARY_VALUE;
    values->data = (stack_value*)((char*)values->data - values->capacity * sizeof(stack_value));

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



void stack_dtor(stack* values)
{
    free(values->data);
    values->size = 0;
    values->capacity     = 0;
    values->hash_struct  = 0;
    values->hash_stack   = 0;
    values->left_canary  = 0;
    values->right_canary = 0;
    return;
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

char* check_args(int argc, char* argv[])
{
    if(argc != 2) {
        printf(".\\stack.exe <name_file>\n");
        exit(1);
    }

    return argv[1];
}

