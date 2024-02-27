#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include "../common/claves.h"

struct vector_node {
    int key;
    char str[MAX_STR_SIZE];
    int n_vec;
    double *vec;
    struct vector_node *next;
};

typedef struct {
    int n_nodes;
    struct vector_node *list;
    pthread_mutex_t m_op;
} linked_list;

linked_list list;
bool initialized = false;

void destroy_node(struct vector_node *n) {
    free((void *)n->vec);
    free((void *)n);
}

void clear() {
    pthread_mutex_lock(&list.m_op);
    while (list.list != NULL) {
        struct vector_node *aux = list.list;
        list.list = list.list->next;
        destroy_node(aux);
    }
    pthread_mutex_unlock(&list.m_op);
    return;
}

int init() {
    if (!initialized) {
        pthread_mutex_init(&list.m_op, NULL);
    }
    clear();
    return 0;
}

int set_value(const int key, const char value1[], const int n_value2, const double v_value2[n_value2]) {
    // Comprobar que el key no se haya creado y que 'N' esté en su rango.
    if (exist(key) == 1 || n_value2 < 1 || n_value2 > 32)
    {
        return -1;
    }

    // sección crítica de operación con la lista.
    pthread_mutex_lock(&list.m_op);

    // guardar el valor del nodo anterior raíz apuntado.
    struct vector_node *aux = list.list;

    // generar el nodo nuevo.
    struct vector_node *node = (struct vector_node *)malloc(sizeof(struct vector_node));

    node->key = key;

    strcpy(node->str, value1);

    node->n_vec = n_value2;
    node->vec = (double *)malloc(n_value2 * sizeof(double));
    for (int i = 0; i < n_value2; i++)
    {
        node->vec[i] = v_value2[i];
    }

    node->next = aux;

    // insertar el nodo generado por la izquierda.
    list.list = node;
    list.n_nodes += 1;

    pthread_mutex_unlock(&list.m_op);

    return 0;
}

int get_value(const int key, char value1[MAX_STR_SIZE], int *n_value2, double v_value2[MAX_VEC_SIZE]) {
    struct vector_node *aux = list.list;

    bool found = false;

    // Sección crítica de operación con la lista.
    pthread_mutex_lock(&list.m_op);

    // busqueda y obtención de valores
    while (aux != NULL)
    {
        if (aux->key == key)
        {
            found = true;
            strcpy(value1, aux->str);
            *n_value2 = aux->n_vec;
            for (int i = 0; i < aux->n_vec; i++)
            {
                v_value2[i] = aux->vec[i];
            }
            break;
        }
        aux = aux->next;
    }

    pthread_mutex_unlock(&list.m_op);

    return found == true ? 0 : -1;
}

int modify_value(const int key, const char value1[], const int n_value2, const double v_value2[n_value2]) {

    struct vector_node *aux = list.list;

    bool found = false;

    // Sección crítica de operación con la lista.
    pthread_mutex_lock(&list.m_op);

    // busqueda y modificación de valores.
    while (aux != NULL)
    {
        if (aux->key == key)
        {
            found = true;
            strcpy(aux->str, value1);
            aux->n_vec = n_value2;
            for (int i = 0; i < aux->n_vec; i++)
            {
                aux->vec[i] = v_value2[i];
            }
            break;
        }
        aux = aux->next;
    }
    pthread_mutex_unlock(&list.m_op);

    return found == true ? 0 : -1;
}

int delete_key(const int key) {
    struct vector_node *aux = NULL;
    struct vector_node *node = list.list;

    bool removed = false;

    // Zona crítica de destrucción de un nodo.
    pthread_mutex_lock(&list.m_op);

    // busqueda y destrucción de un nodo con una clave especificada.
    while (node != NULL)
    {
        if (node->key == key && aux == NULL)
        {
            aux = node;
            list.list = node->next;
            destroy_node(aux);
            removed = true;
            list.n_nodes -= 1;
            break;
        }
        else if (node->key == key)
        {
            aux->next = node->next;
            destroy_node(node);
            removed = true;
            list.n_nodes -= 1;
            break;
        }

        aux = node;
        node = node->next;
    }

    pthread_mutex_unlock(&list.m_op);

    return removed == true ? 0 : -1;
}

int exist(const int key) {
    struct vector_node *aux = list.list;

    bool found = false;

    // Sección crítica de operación con la lista.
    pthread_mutex_lock(&list.m_op);

    // busqueda y obtención de valores
    while (aux != NULL)
    {
        if (aux->key == key)
        {
            found = true;
            break;
        }
        aux = aux->next;
    }

    pthread_mutex_unlock(&list.m_op);

    return found == true ? 1 : 0;
}

void destroy() {
    clear();
    pthread_mutex_destroy(&list.m_op);
}
