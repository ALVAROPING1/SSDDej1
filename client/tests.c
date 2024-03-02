#include <stdio.h>

int intcmp(int v1, int v2) {
    return v1 != v2;
}

void print_vec(int size, double vec[size]) {
    printf("[");
    if (size > 0) { printf("%g", vec[0]); }
    for (int i = 1; i < size; ++i) {
        printf(", %g", vec[i]);
    }
    printf("]\n");
}
