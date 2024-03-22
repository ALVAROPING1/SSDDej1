#include <arpa/inet.h>
#include <mqueue.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/claves.h"
#include "../common/comm.h"
#include "../common/error_handling.h"
#include "../common/messages.h"
#include "../common/env.h"

#undef FMT_ERROR
#define FMT_ERROR(msg) ERROR("CONNECTION ERROR", msg)

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║                             Server Connection                             ║
// ╚═══════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Sends a request to the server and receives the result
 *
 * @param sd Socket to use for the connection
 * @param request Request to send
 * @param response Buffer in which the response should be stored
 * @return Error code of the communication
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
static int send_message(int sd, const Request* request, Response* response) {
    char op_code = (char) request->op_code;
    CHECK(sendMessage(sd, 1, &op_code), "Error sending op_code");
    CHECK(send_int(sd, request->key), "Error sending key");
    CHECK(send_value(sd, request->str, request->vec_size, request->vec));
    CHECK(recv_int(sd, &response->ret_value), "Error receiving return value");
    CHECK(recv_value(sd, response->str, &response->vec_size, response->vec));
    return 0;
}

/**
 * @brief Makes a request to the server and receives the result
 *
 * @param request Request to send
 * @param response Buffer in which the response should be stored
 * @return Error code of the request
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
static int make_request(Request* request, Response* response) {
    struct sockaddr_in server_addr; 
    int port;
    char server_ip[MAX_STR_SIZE];  

    int sd = CHECK(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),
                   "Error creating client socket",
                   -1);

    // Obtain Server address
    bzero((char*) &server_addr, sizeof(server_addr)); 
    CHECK(getServerIp(server_ip),"Cannot get server IP"); // Get server IP
    CHECK(getServerPort(&port),"Cannot get server port"); // Get server PORT

    server_addr.sin_addr.s_addr = inet_addr(server_ip); 
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(port);

    CHECK(connect(sd, (struct sockaddr*) &server_addr, sizeof(server_addr)),
          "Error connecting to server");

    if (send_message(sd, request, response) == -1) { response->ret_value = -1; }

    CHECK(close(sd), "Error closing client socket");
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

#undef FMT_ERROR
#define FMT_ERROR(msg) ERROR("REQUEST PROCESSING ERROR", msg)

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
