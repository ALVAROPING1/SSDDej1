#ifndef COMM_H
#define COMM_H

#include <stdio.h>

#include "claves.h"

int recvMessage(const int fd, const size_t size, void* data);

int sendMessage(const int fd, const size_t size, const void* data);

ssize_t readLine(const int fd, void* buffer, const size_t n);

int send_int(const int fd, const int val);

int recv_int(const int fd, int* val);

int send_value(const int fd,
               const char str[],
               const int vec_size,
               const double vec[vec_size]);

int recv_value(const int fd,
               char str[MAX_STR_SIZE],
               int* vec_size,
               double vec[MAX_VEC_SIZE]);

#endif // COMM_H
