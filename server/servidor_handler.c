#include <mqueue.h>
#include <stdbool.h>

#include "../common/claves.h"
#include "../common/error_handling.h"
#include "servidor_handler.h"

/**
 * Executes a request
 *
 * @param req Request to execute
 * @param res Result of the execution of the request
 * @return Error code generated during the execution of the request
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

int handle_request(const Request* request) {
    Response response;
    const struct mq_attr q_attr = {.mq_msgsize = sizeof(Response),
                                   .mq_maxmsg = 10};
    response.ret_value = execute_request(request, &response);
    mqd_t queue =
        CHECK(mq_open(request->q_name, O_CREAT | O_WRONLY, 0700, &q_attr),
              "Error opening response message queue",
              -1);
    CHECK(mq_send(queue, (const char*) &response, sizeof(response), 1),
          "Error sending response message");
    CHECK(mq_close(queue), "Error closing response queue");
    return 0;
};
