#ifndef MESSAGES_H
#define MESSAGES_H

#include "claves.h"

typedef enum { Init, Delete, Exist, Get, Set, Modify } OpCode;

typedef struct {
    OpCode op_code;
    int key, vec_size;
    char str[MAX_STR_SIZE];
    double vec[MAX_VEC_SIZE];
    char q_name[32];
} Request;

typedef struct {
    int ret_value, vec_size;
    char str[MAX_STR_SIZE];
    double vec[MAX_VEC_SIZE];
} Response;

#endif // MESSAGES_H
