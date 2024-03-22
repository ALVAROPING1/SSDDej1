#include <mqueue.h>
#include <stdbool.h>
#include <stdio.h>

#include "../common/claves.h"
#include "../common/comm.h"
#include "../common/error_handling.h"
#include "servidor_handler.h"

#undef FMT_ERROR
#define FMT_ERROR(msg) ERROR("REQUEST PROCESSING ERROR", msg)

/**
 * @brief Executes a request
 *
 * @param req Request to execute
 * @param res Result of the execution of the request
 * @return Error code generated during the execution of the request
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int execute_request(const Request* req, Response* res) {
    // no es zona crítica ya que las operaciones se realizan de forma atómica
    // debido al mutex incorporado en la lista enlazada.
    switch (req->op_code) {
    case Init:
        return init();
    case Delete:
        return delete_key(req->key);
    case Exist:
        return exist(req->key);
    case Get:
        return get_value(req->key, res->str, &res->vec_size, res->vec);
    case Set:
        return set_value(req->key, req->str, req->vec_size, req->vec);
    case Modify:
        return modify_value(req->key, req->str, req->vec_size, req->vec);
    }
    ASSERT(false, "Unknown operation code");
}

#undef FMT_ERROR
#define FMT_ERROR(msg) ERROR("CONNECTION ERROR", msg)

int handle_request(int sd, const Request* request) {
    printf("request: opcode: %d, key: %d\n", request->op_code, request->key);
    Response response = {0};
    response.ret_value = execute_request(request, &response);
    printf("result: %d\n", response.ret_value);
    CHECK(send_int(sd, response.ret_value), "Error sending return value");
    CHECK(send_value(sd, response.str, response.vec_size, response.vec));
    CHECK(close(sd), "Error closing client socket");
    return 0;
};
