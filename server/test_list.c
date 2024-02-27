#include <stdio.h>
#include <string.h>
#include "linked_list.h"

int main(int argc, char const *argv[])
{
    linked_list *lista1 = init_list();

    double vec1[2] = {1, 2};

    set_value(lista1, 0, "hola", 2, vec1);

    if (lista1->list != NULL)
    {
        printf("Exito");
    }
    else
    {
        printf("Fracaso");
    }

    int n_vec;
    char val1[256];
    double vec2[256];

    get_value(lista1, 0, val1, &n_vec, vec2);

    if (strcmp(val1, "hola") == 0)
    {
        printf("Exito 2!");
    }
    else
    {
        printf("Fracaso 2!");
    }

    for (int i = 0; i < 2; i++)
    {
        printf("Componente %d: %f", i, lista1->list->vec[i]);
    }

    clear(lista1);

    if (lista1->list == NULL)
    {
        printf("Exito 3!");
    }
    else
    {
        printf("Fracaso 3!");
    }

    destroy(lista1);

    return 0;
}
