#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define MAX_KEY_LENGTH 256
#include <pthread.h>

struct vector_node
{
    int key;
    char str[MAX_KEY_LENGTH];
    int n_vec;
    double *vec;
    struct vector_node *next;
};

typedef struct
{
    int n_nodes;
    struct vector_node *list;
    pthread_mutex_t m_op;
} linked_list;

linked_list *init_list();
int set_value(linked_list *l, int key, char *value1, int n_value2, double *v_value2);
int get_value(linked_list *l, int key, char *value1, int *n_value2, double *v_value2);
int modify_value(linked_list *l, int key, char *value1, int n_value2, double *v_value2);
int delete_key(linked_list *l, int key);
int exist(linked_list *l, int key);
void clear(linked_list *l);
void destroy(linked_list *l);
void destroy_node(struct vector_node *n);
#endif // !LINKED_LIS_HT