#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <unistd.h>

#include "../common/comm.h"
#include "../common/error_handling.h"
#include "../common/utils.h"
#include "servidor_handler.h"

int receive_request(int sd, Request* request) {
    CHECK(recvMessage(sd, 1, &request->op_code), "Error receiving op_code");
    CHECK(recv_int(sd, &request->key), "Error receiving key");
    CHECK(recv_value(sd, request->str, &request->vec_size, request->vec));
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
        error = true;                                                          \
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
int main(int argc, char* argv[]) {
    // Parameter checking.
    if (argc != 2) {
        printf("Usage: ./%s <port>.", argv[0]);
        return EXIT_FAILURE;
    };

    int port;
    CHECK(string2int(argv[1], &port), "Error converting the port to a number");

    if (signal(SIGINT, siginthandler) == SIG_ERR) {
        perror(WARN("Error setting `SIGINT` handler"));
    };

    sd = CHECK(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),
               "Error creating server socket",
               -1);

    char val = 1;
    CHECK(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)),
          "Error setting reuse address option");

    struct sockaddr_in server_addr = {0}, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
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
        socklen_t size = sizeof(struct sockaddr_in);
        int newsd = accept(sd, (struct sockaddr*) &client_addr, &size);
        if (newsd == -1) {
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
