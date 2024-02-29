#ifndef SERVIDOR_HANDLER_H
#define SERVIDOR_HANDLER_H

#include "../common/messages.h"

/**
 * Handles a request
 *
 * @param request Request to process
 * @return Error code of the processing of the request
 */
int handle_request(const Request* request);

#endif // SERVIDOR_HANDLER_H
