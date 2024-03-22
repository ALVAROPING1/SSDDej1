#ifndef SERVIDOR_HANDLER_H
#define SERVIDOR_HANDLER_H

#include "../common/messages.h"

/**
 * @brief Handles a request
 *
 * @param sd Socket to use for the connection
 * @param request Request to process
 * @return Error code of the processing of the request
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int handle_request(int sd, const Request* request);

#endif // SERVIDOR_HANDLER_H
