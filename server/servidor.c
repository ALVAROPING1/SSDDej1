#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/error_handling.h"
#include "servidor_handler.h"

// Copy request mutex
pthread_mutex_t m_copy;
// Copy request condition
pthread_cond_t c_copy;
// Copy request condition variable
bool copied = false;

/**
 * @brief Setup function for processing a request in a new thread
 *
 * @param request Request to process. Should be a pointer to a `Request` struct
 * @return Error code of the processing of the request
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
void* setup_request(void* request) {
    Request req;
    memcpy(&req, request, sizeof(req));
    // Zona de copia de mensajes
    if (pthread_mutex_lock(&m_copy) != 0) {
        perror(ERROR("Error locking copy mutex on worker thread"));
        return (void*) -1;
    }
    copied = true;
    pthread_cond_signal(&c_copy);
    if (pthread_mutex_unlock(&m_copy) != 0) {
        perror(ERROR("Error unlocking copy mutex on worker thread"));
        return (void*) -1;
    }
    return (void*) (long) handle_request(&req);
}

mqd_t queue;
pthread_attr_t t_attr;

#define TRY(expr, err_msg)                                                     \
    if ((expr) != 0) {                                                         \
        perror(WARN(err_msg));                                                 \
        error = false;                                                         \
    }

/**
 * @brief SIGINT Handler for closing the server
 */
void siginthandler() {
    printf("\n\033[32;1m[SERVER CLOSED]\033[0m\n");
    // Cerrar las estructuras de datos.
    bool error = false;
    TRY(mq_close(queue), "Error closing requests message queue");
    TRY(pthread_attr_destroy(&t_attr),
        "Error destroying thread attributes struct");
    TRY(pthread_mutex_destroy(&m_copy), "Error destroying copy mutex");
    TRY(pthread_cond_destroy(&c_copy), "Error destroying copy condition");
    exit(error);
}

/**
 * @brief Server entry point
 *
 * @return Error code of the execution if there was an irrecoverable error
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int main() {
    // Inicializar mutex de copia y condition
    pthread_mutex_init(&m_copy, NULL);
    pthread_cond_init(&c_copy, NULL);
    if (signal(SIGINT, siginthandler) == SIG_ERR) {
        perror(WARN("Error setting `SIGINT` handler"));
    };

    CHECK(pthread_attr_init(&t_attr),
          ERROR("Error creating thread attributes struct"));
    CHECK(pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED),
          ERROR("Error setting detach state on thread attributes struct"));

    Request request;
    const struct mq_attr q_attr = {.mq_msgsize = sizeof(Request),
                                   .mq_maxmsg = 10};
    queue = CHECK(mq_open("/SERVIDOR", O_CREAT | O_RDONLY, 0700, &q_attr),
                  ERROR("Error creating requests message queue"),
                  -1);

    printf("\033[32;1m[SERVER STARTED]\033[0m\n");

    // Bucle de peticiones
    while (true) {
        if (mq_receive(queue, (char*) &request, sizeof(request), NULL) == -1) {
            perror(WARN("Error receiving message"));
            continue;
        }
        printf("Request received\n");
        pthread_t thread;
        if (pthread_create(&thread, &t_attr, setup_request, (void*) &request) !=
            0) {
            perror(WARN("Error creating thread to handle the request"));
            continue;
        }

        CHECK(pthread_mutex_lock(&m_copy),
              ERROR("Error locking copy mutex on main thread"));
        while (!copied) {
            pthread_cond_wait(&c_copy, &m_copy);
        }
        copied = false;
        CHECK(pthread_mutex_unlock(&m_copy),
              ERROR("Error unlocking copy mutex on main thread"));
    }

    return EXIT_SUCCESS;
}
