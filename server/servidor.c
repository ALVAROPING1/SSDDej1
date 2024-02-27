#include <mqueue.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/error_handling.h"
#include "servidor_handler.h"

pthread_mutex_t m_copy;
bool copied = false;
pthread_cond_t c_copy;

void* setup_request(void* request) {
    Request req;
    memcpy(&req, request, sizeof(req));
    // Zona de copia de mensajes
    pthread_mutex_lock(&m_copy);
    copied = true;
    pthread_cond_signal(&c_copy);
    pthread_mutex_unlock(&m_copy);
    return (void*) (long) handle_request(&req);
}

int main(int argc, char* argv[]) {
    // Inicializar mutex de copia y condition
    pthread_mutex_init(&m_copy, NULL);
    pthread_cond_init(&c_copy, NULL);

    pthread_attr_t t_attr;
    CHECK(pthread_attr_init(&t_attr),
          "Error creating thread attributes struct");
    CHECK(pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED),
          "Error setting detach state on thread attributes struct");

    Request request;
    const struct mq_attr q_attr = {.mq_msgsize = sizeof(Request),
                                   .mq_maxmsg = 10};
    mqd_t queue = CHECK(mq_open("/SERVIDOR", O_CREAT | O_RDONLY, 0700, &q_attr),
                        "Error creating requests message queue",
                        -1);

    printf("\033[32;1m[SERVER STARTED]\033[0m\n");

    // Bucle de peticiones
    while (true) {
        if (mq_receive(queue, (char*) &request, sizeof(request), NULL) == -1) {
            perror("Error receiving message");
            continue;
        }
        printf("Request received\n");
        pthread_t thread;
        if (pthread_create(&thread, &t_attr, setup_request, (void*) &request) !=
            0) {
            perror("Error creating thread to handle the request");
        }

        CHECK(pthread_mutex_lock(&m_copy), "Error locking copy mutex");
        while (!copied) {
            pthread_cond_wait(&c_copy, &m_copy);
        }
        copied = false;
        CHECK(pthread_mutex_unlock(&m_copy), "Error unlocking copy mutex");
    }

    return EXIT_SUCCESS;
}

// int main(int argc, char const *argv[])
// {
//   // cerrar las estructuras de datos.
//   destroy(list);
//   mq_close(mq_pet);
//   pthread_attr_destroy(&atts);
//
//   //destruir todos los mutex y conditions.
//   pthread_mutex_destroy(&m_copy);
//   pthread_cond_destroy(&c_copy);
//   return 0;
// }
