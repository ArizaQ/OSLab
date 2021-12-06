#define  ORIGIN_CAPACITY 2500;
#ifndef _ORANGES_VECTOR_H_
#define _ORANGES_VECTOR_H_

/* CONSOLE */
typedef struct vector
{
	int capacity; 
    int size;
    int* tail;
    int* head;
    int array[2500];
}VECTOR;

typedef struct opration_t{
    int opcode;
    int oprand;
} OPERATION;
typedef struct vector_operation{
    int capacity; 
    int size;
    OPERATION* tail;
    OPERATION* head;
    OPERATION array[2500];
} VECTOR_OPRATION;


#endif
