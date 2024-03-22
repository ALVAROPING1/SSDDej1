#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../common/comm.h"
#include "../common/error_handling.h"
#include "servidor_handler.h"

int receive_request(int sd, Request* request) {
    CHECK(recvMessage(sd, 1, &request->op_code), "Error receiving op_code");
    CHECK(recv_int(sd, &request->key), "Error receiving key");
    CHECK(recv_value(sd, request->str, &request->vec_size, request->vec),
          "Error receiving values");
    return 0;
}

/**
 * @brief Setup function for processing a request in a new thread
 *
 * @param request Request to process. Should be a pointer to a `Request` struct
 * @return Error code of the processing of the request
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
void* setup_request(void* request) {
    int sd = (size_t) request;
    Request req;
    if (receive_request(sd, &req) != 0) { return (void*) -1; }
    return (void*) (long) handle_request(sd, &req);
}

int sd;
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
    TRY(pthread_attr_destroy(&t_attr),
        "Error destroying thread attributes struct");
    TRY(close(sd), "Error closing server socket");
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
    // pthread_mutex_init(&m_copy, NULL);
    // pthread_cond_init(&c_copy, NULL);
    // if (signal(SIGINT, siginthandler) == SIG_ERR) {
    //     perror(WARN("Error setting `SIGINT` handler"));
    // };
    //
    // CHECK(pthread_attr_init(&t_attr),
    //       "Error creating thread attributes struct");
    // CHECK(pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED),
    //       "Error setting detach state on thread attributes struct");
    //
    // Request request;
    // queue = CHECK(mq_open("/SERVIDOR", O_CREAT | O_RDONLY, 0700, &q_attr),
    //               "Error creating requests message queue",
    //               -1);
    //
    //
    // // Bucle de peticiones
    // while (true) {
    //     if (mq_receive(queue, (char*) &request, sizeof(request), NULL) == -1)
    //     {
    //         perror(WARN("Error receiving message"));
    //         continue;
    //     }
    //     pthread_t thread;
    //     if (pthread_create(&thread, &t_attr, setup_request, (void*) &request)
    //     !=
    //         0) {
    //         perror(WARN("Error creating thread to handle the request"));
    //         continue;
    //     }
    //
    //     CHECK(pthread_mutex_lock(&m_copy),
    //           "Error locking copy mutex on main thread");
    //     while (!copied) {
    //         pthread_cond_wait(&c_copy, &m_copy);
    //     }
    //     copied = false;
    //     CHECK(pthread_mutex_unlock(&m_copy),
    //           "Error unlocking copy mutex on main thread");
    // }
    //
    // return EXIT_SUCCESS;
    int newsd;
    int val;
    socklen_t size;

    sd = CHECK(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),
               "Error creating server socket",
               -1);

    val = 1;
    CHECK(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*) &val, sizeof(int)),
          "Error setting reuse address option");

    struct sockaddr_in server_addr, client_addr;
    bzero((char*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000); // TODO:
    server_addr.sin_addr.s_addr = INADDR_ANY;

    CHECK(bind(sd, (struct sockaddr*) &server_addr, sizeof(server_addr)),
          "Error binding server socket");
    CHECK(listen(sd, SOMAXCONN), "Error setting listen socket");

    pthread_attr_t t_attr;
    CHECK(pthread_attr_init(&t_attr),
          "Error creating thread attributes struct");
    CHECK(pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED),
          "Error setting detach state on thread attributes struct");

    printf("\033[32;1m[SERVER STARTED]\033[0m\n");

    while (1) {
        size = sizeof(struct sockaddr_in);
        newsd = accept(sd, (struct sockaddr*) &client_addr, &size);
        if (newsd < 0) {
            perror(WARN("Error en el accept"));
            continue;
        }

        pthread_t thread;
        printf("Conexión aceptada de IP: %s y puerto: %d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        if (pthread_create(
                &thread, &t_attr, setup_request, (void*) (long) newsd) != 0) {
            perror(WARN("Error creating thread to handle connection"));
        }
    }

    return EXIT_SUCCESS;
}
