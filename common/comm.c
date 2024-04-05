#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "claves.h"
#include "comm.h"
#include "error_handling.h"
#include "utils.h"

#define INT_MAX_SIZE 16
#define DBL_MAX_SIZE 32

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                           Connection Primitives                           ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Performs a fixed size message operation (send/receive)
 *
 * @param sd Socket file descriptor
 * @param size Size in bytes of the message
 * @param data Buffer where the message should be stored/written
 * @param operation Function performing the operation. Should be `write` or
 *     `read`
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int msg_operation(const int sd,
                  const size_t size,
                  void* data,
                  ssize_t (*operation)(int, void*, size_t)) {
    int res;
    int remaining = size;

    do {
        res = operation(sd, data, size);
        remaining = remaining - res;
        data = data + res;
    } while ((remaining > 0) && (res >= 0));

    CHECK(res == -1);
    return 0;
}

int recvMessage(const int sd, const size_t size, void* data) {
    return msg_operation(sd, size, data, read);
}

int sendMessage(const int sd, const size_t size, const void* data) {
    return msg_operation(sd, size, (void*) data, (ssize_t(*)()) write);
}

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
ssize_t readLine(const int sd, void* buffer, const size_t n) {
    ssize_t numRead; /* num of bytes fetched by last read() */
    size_t totRead;  /* total bytes read so far */
    char* buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;
    totRead = 0;

    for (;;) {
        numRead = read(sd, &ch, 1); /* read a byte */

        if (numRead == -1) {
            if (errno == EINTR) continue; /* interrupted -> restart read() */
            else return -1;               /* some other error */
        } else if (numRead == 0) {        /* EOF */
            if (totRead == 0) return 0;   /* no byres read; return 0 */
            else break;
        } else { /* numRead must be 1 if we get here*/
            if (ch == '\0' || ch == '\n') break;
            if (totRead < n - 1) { /* discard > (n-1) bytes */
                totRead++;
                *buf++ = ch;
            }
        }
    }

    *buf = '\0';
    return totRead;
}

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                           Send Data Operations                            ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

int send_int(const int sd, const int val) {
    char buf[INT_MAX_SIZE];
    snprintf(buf, sizeof(buf), "%d", val);
    CHECK(sendMessage(sd, strlen(buf) + 1, buf));
    return 0;
}

int recv_int(const int sd, int* val) {
    char buf[INT_MAX_SIZE] = {};
    CHECK(readLine(sd, buf, sizeof(buf)) == -1);
    CHECK(string2int(buf, val));
    return 0;
}

/**
 * @brief Sends a vector of doubles as strings
 *
 * @param sd Socket file descriptor
 * @param len Size of the vector
 * @param vec Buffer with the vector
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int send_vector(const int sd, int len, const double vec[len]) {
    char buf[DBL_MAX_SIZE + 1];
    for (int i = 0; i < len; i++) {
        snprintf(buf, sizeof(buf), "%.17le", vec[i]);
        CHECK(sendMessage(sd, strlen(buf) + 1, buf),
              "Error sending vector component");
    }
    return 0;
}

/**
 * @brief Receives a vector of doubles as strings
 *
 * @param sd Socket file descriptor
 * @param len Expected size of the vector
 * @param vec Buffer in which the vector should be stored
 * @return Error code of the execution
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int recv_vector(const int sd, int len, double vec[len]) {
    char buf[DBL_MAX_SIZE + 1];
    for (int i = 0; i < len; i++) {
        CHECK(readLine(sd, buf, sizeof(buf)) == -1,
              "Error receiving vector component");
        sscanf(buf, "%le", &vec[i]);
    }
    return 0;
}

int send_value(const int sd,
               const char str[],
               const int vec_size,
               const double vec[vec_size]) {
    CHECK(sendMessage(sd, strlen(str) + 1, str), "Error sending string");
    CHECK(send_int(sd, vec_size), "Error sending vector size");
    CHECK(send_vector(sd, vec_size, vec));
    return 0;
}

int recv_value(const int sd,
               char str[MAX_STR_SIZE],
               int* vec_size,
               double vec[MAX_VEC_SIZE]) {
    CHECK(readLine(sd, str, MAX_STR_SIZE), "Error receiving string", -1);
    CHECK(recv_int(sd, vec_size), "Error receiving vector size");
    CHECK(recv_vector(sd, *vec_size, vec));
    return 0;
}
