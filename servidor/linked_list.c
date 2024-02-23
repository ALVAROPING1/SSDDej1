#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

linked_list *init_list()
{
    linked_list *l = (linked_list *)malloc(sizeof(linked_list));
    pthread_mutex_init(&l->m_op, NULL);
    l->n_nodes = 0;
    l->list = NULL;

    return l;
}

int set_value(linked_list *l, int key, char *value1, int n_value2, double *v_value2)
{
    // Comprobar que el key no se haya creado y que 'N' esté en su rango.
    if (exist(l, key) == 1 || n_value2 < 1 || n_value2 > 32)
    {
        return -1;
    }

    // sección crítica de operación con la lista.
    pthread_mutex_lock(&l->m_op);

    // guardar el valor del nodo anterior raíz apuntado.
    struct vector_node *aux = l->list;

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
    l->list = node;
    l->n_nodes += 1;

    pthread_mutex_unlock(&l->m_op);

    return 0;
}

int get_value(linked_list *l, int key, char *value1, int *n_value2, double *v_value2)
{
    struct vector_node *aux = l->list;

    bool found = false;

    // Sección crítica de operación con la lista.
    pthread_mutex_lock(&l->m_op);

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

    pthread_mutex_unlock(&l->m_op);

    return found == true ? 0 : -1;
}

int modify_value(linked_list *l, int key, char *value1, int n_value2, double *v_value2)
{

    struct vector_node *aux = l->list;

    bool found = false;

    // Sección crítica de operación con la lista.
    pthread_mutex_lock(&l->m_op);

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
    pthread_mutex_unlock(&l->m_op);

    return found == true ? 0 : -1;
}

int delete_key(linked_list *l, int key)
{
    struct vector_node *aux = NULL;
    struct vector_node *node = l->list;

    bool removed = false;

    // Zona crítica de destrucción de un nodo.
    pthread_mutex_lock(&l->m_op);

    // busqueda y destrucción de un nodo con una clave especificada.
    while (node != NULL)
    {
        if (node->key == key && aux == NULL)
        {
            aux = node;
            l->list = node->next;
            destroy_node(aux);
            removed = true;
            l->n_nodes -= 1;
            break;
        }
        else if (node->key == key)
        {
            aux->next = node->next;
            destroy_node(node);
            removed = true;
            l->n_nodes -= 1;
            break;
        }

        aux = node;
        node = node->next;
    }

    pthread_mutex_unlock(&l->m_op);

    return removed == true ? 0 : -1;
}

int exist(linked_list *l, int key)
{
    struct vector_node *aux = l->list;

    bool found = false;

    // Sección crítica de operación con la lista.
    pthread_mutex_lock(&l->m_op);

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

    pthread_mutex_unlock(&l->m_op);

    return found == true ? 1 : 0;
}

void clear(linked_list *l)
{
    pthread_mutex_lock(&l->m_op);
    while (l->list != NULL)
    {
        struct vector_node *aux = l->list;
        l->list = l->list->next;
        destroy_node(aux);
    }
    pthread_mutex_unlock(&l->m_op);
    return;
}

void destroy(linked_list *l)
{
    clear(l);
    pthread_mutex_destroy(&l->m_op);
    free((void *)l);
}

void destroy_node(struct vector_node *n)
{
    free((void *)n->vec);
    n->next = NULL;
    strcpy(n->str, "");
    n->n_vec = 0;
    n->key = 0;
    free((void *)n);
}