#include <mqueue.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "../common/claves.h"
#include "../common/error_handling.h"
#include "../common/messages.h"

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                             Server Connection                             ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Sends a request to the server and receives the result
 *
 * @param request Request to send
 * @param response Buffer in which the response should be stored
 * @param q_server Server message queue
 * @param q_res Response message queue
 * @return Error code
 */
int send_message(const Request* request,
                 Response* response,
                 const mqd_t q_server,
                 const mqd_t q_res) {
    CHECK(mq_send(q_server, (char*) request, sizeof(*request), 0),
          "Error sending message",
          -1);
    CHECK(mq_receive(q_res, (char*) response, sizeof(*response), NULL),
          "Error receiving message",
          -1);
    return 0;
}

#define TRY(expr, err_msg)                                                     \
    if ((expr) == -1) { perror(err_msg); }

/**
 * @brief Makes a request to the server and receives the result
 *
 * @param request Request to send
 * @param response Buffer in which the response should be stored
 * @return Error code
 */
int make_request(Request* request, Response* response) {
    snprintf(request->q_name, sizeof(request->q_name), "/CLIENT_%d", getpid());

    const struct mq_attr q_attr = {.mq_msgsize = sizeof(Response),
                                   .mq_maxmsg = 10};

    int q_server = CHECK(mq_open("/SERVIDOR", O_WRONLY),
                         "Error opening server message queue",
                         -1);
    int q_res = mq_open(request->q_name, O_CREAT | O_RDONLY, 0700, &q_attr);

    if (q_res == -1) {
        perror("Error creating response message queue");
        CHECK(mq_close(q_server), "Error closing server message queue");
        return -1;
    }

    if (send_message(request, response, q_server, q_res) == -1) {
        response->ret_value = -1;
    }

    TRY(mq_close(q_server), "Error closing server message queue");
    TRY(mq_close(q_res), "Error closing response message queue");
    TRY(mq_unlink(request->q_name), "Error deleting response message queue");
    return response->ret_value;
}

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                            API Implementation                             ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

int init() {
    Request request = {.op_code = Init};
    Response response;
    return make_request(&request, &response);
}

int delete_key(const int key) {
    Request request = {.op_code = Delete, .key = key};
    Response response;
    return make_request(&request, &response);
}

int exist(const int key) {
    Request request = {.op_code = Exist, .key = key};
    Response response;
    return make_request(&request, &response);
}

int get_value(const int key,
              char str[MAX_STR_SIZE],
              int* vec_size,
              double vec[MAX_VEC_SIZE]) {
    Request request = {.op_code = Get, .key = key};
    Response response;
    CHECK(make_request(&request, &response));
    strcpy(str, response.str);
    *vec_size = response.vec_size;
    memcpy(vec, response.vec, sizeof(*vec) * response.vec_size);
    return 0;
}

int set_value(const int key,
              const char string[],
              const int vec_size,
              const double vec[vec_size]) {
    ASSERT(vec_size >= 1 && vec_size <= MAX_VEC_SIZE,
           "Invalid vector size %d",
           vec_size);
    int str_size = strlen(string);
    ASSERT(str_size <= MAX_STR_SIZE - 1, "Invalid string size %d", str_size)
    Request request = {.op_code = Set, .key = key, .vec_size = vec_size};
    Response response;
    strcpy(request.str, string);
    memcpy(request.vec, vec, sizeof(*vec) * vec_size);
    return make_request(&request, &response);
}

int modify_value(const int key,
                 const char string[],
                 const int vec_size,
                 const double vec[vec_size]) {
    ASSERT(vec_size >= 1 && vec_size <= MAX_VEC_SIZE,
           "Invalid vector size %d",
           vec_size);
    int str_size = strlen(string);
    ASSERT(str_size <= MAX_STR_SIZE - 1, "Invalid string size %d", str_size)
    Request request = {.op_code = Modify, .key = key, .vec_size = vec_size};
    Response response;
    strcpy(request.str, string);
    memcpy(request.vec, vec, sizeof(*vec) * vec_size);
    return make_request(&request, &response);
}
