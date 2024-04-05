#ifndef COMM_H
#define COMM_H

#include <stdio.h>

#include "claves.h"

/**
 * @brief Receives a fixed length message as a byte array
 *
 * @param sd Socket file descriptor
 * @param size Size in bytes of the message
 * @param data Buffer where the message should be stored
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int recvMessage(const int sd, const size_t size, void* data);

/**
 * @brief Sends a fixed length message as a byte array
 *
 * @param sd Socket file descriptor
 * @param size Size in bytes of the message
 * @param data Buffer with the message
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int sendMessage(const int sd, const size_t size, const void* data);

/**
 * @brief Receives a variable length message terminated by a null terminator
 * (`\0`) or a new line character (`\n`). The terminator byte isn't returned
 *
 * @param sd Socket file descriptor
 * @param buffer Buffer where the message should be stored
 * @param n Size of the buffer. Further bytes will be discarded
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
ssize_t readLine(const int sd, void* buffer, const size_t n);

/**
 * @brief Receives an int sent as a string
 *
 * @param sd Socket file descriptor
 * @param val Memory location where the int should be stored
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int recv_int(const int sd, int* val);

/**
 * @brief Sends an int as a string
 *
 * @param sd Socket file descriptor
 * @param val Value to send
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int send_int(const int sd, const int val);

/**
 * @brief Receives the values of a tuple
 *
 * @param sd Socket file descriptor
 * @param str Buffer where the string value should be stored
 * @param vec_size Memory location where the vector size should be stored
 * @param vec Buffer where the vector value should be stored
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int recv_value(const int sd,
               char str[MAX_STR_SIZE],
               int* vec_size,
               double vec[MAX_VEC_SIZE]);

/**
 * @brief Sends the values of a tuple
 *
 * @param sd Socket file descriptor
 * @param str Buffer with the string value
 * @param vec_size Size of the vector
 * @param vec Buffer with the vector value
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int send_value(const int sd,
               const char str[],
               const int vec_size,
               const double vec[vec_size]);

#endif // COMM_H
