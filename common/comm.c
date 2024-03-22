#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "claves.h"
#include "error_handling.h"

int msg_operation(const int fd,
                  const size_t size,
                  void* data,
                  ssize_t (*operation)(int, void*, size_t)) {
    int res;
    int remaining = size;

    do {
        res = operation(fd, data, size);
        remaining = remaining - res;
        data = data + res;
    } while ((remaining > 0) && (res >= 0));

    CHECK(res < 0);
    return 0;
}

int recvMessage(const int fd, const size_t size, void* data) {
    return msg_operation(fd, size, data, read);
}

int sendMessage(const int fd, const size_t size, const void* data) {
    return msg_operation(fd, size, (void*) data, (ssize_t(*)()) write);
}

ssize_t readLine(const int fd, void* buffer, const size_t n) {
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
        numRead = read(fd, &ch, 1); /* read a byte */

        if (numRead == -1) {
            if (errno == EINTR) /* interrupted -> restart read() */
                continue;
            else return -1;        /* some other error */
        } else if (numRead == 0) { /* EOF */

            if (totRead == 0) /* no byres read; return 0 */
                return 0;
            else break;
        } else { /* numRead must be 1 if we get here*/
            if (ch == '\n') break;
            if (ch == '\0') break;
            if (totRead < n - 1) { /* discard > (n-1) bytes */
                totRead++;
                *buf++ = ch;
            }
        }
    }

    *buf = '\0';
    return totRead;
}

int send_int(const int fd, const int val) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", val);
    CHECK(sendMessage(fd, strlen(buf) + 1, buf));
    return 0;
}

int recv_int(const int fd, int* val) {
    char buf[16] = {};
    CHECK(readLine(fd, buf, sizeof(buf)) < 0);
    *val = atoi(buf);
    return 0;
}

int send_value(const int fd,
               const char str[],
               const int vec_size,
               const double vec[vec_size]) {
    CHECK(sendMessage(fd, strlen(str) + 1, str), "Error sending string");
    CHECK(send_int(fd, vec_size), "Error writing vector size");
    CHECK(sendMessage(fd, vec_size * sizeof(*vec), vec),
          "Error writing vector");
    return 0;
}

int recv_value(const int fd,
               char str[MAX_STR_SIZE],
               int* vec_size,
               double vec[MAX_VEC_SIZE]) {
    CHECK(readLine(fd, str, MAX_STR_SIZE), "Error reading string", -1);
    CHECK(recv_int(fd, vec_size), "Error reading vector size");
    CHECK(recvMessage(fd, *vec_size * sizeof(*vec), vec),
          "Error reading vector");
    return 0;
}
