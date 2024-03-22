#ifndef MESSAGES_H
#define MESSAGES_H

#include "claves.h"

/**
 * Request operation code
 */
typedef enum { Init, Delete, Exist, Get, Set, Modify } OpCode;

/**
 * Message request structure
 */
typedef struct Request {
    OpCode op_code;           // Operation code
    int key;                  // Data key
    int vec_size;             // Vector size
    char str[MAX_STR_SIZE];   // String value
    double vec[MAX_VEC_SIZE]; // Vector value
    // char q_name[MAX_STR_SIZE]; // Response queue name
} Request;

/**
 * Message response structure
 */
typedef struct Response {
    int ret_value;            // Return value
    char str[MAX_STR_SIZE];   // String value
    int vec_size;             // Vector size
    double vec[MAX_VEC_SIZE]; // Vector value
} Response;

#endif // MESSAGES_H
