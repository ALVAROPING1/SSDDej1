#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <string.h>

#define TEST(name)                                                             \
    int test_##name();                                                         \
    void name() {                                                              \
        printf(test_##name() == 0 ? "\033[32;1m[SUCCESS]"                      \
                                  : "\033[31;1m[FAILURE]");                    \
        printf("\033[0m " #name "\n");                                         \
    }                                                                          \
    int test_##name()

#define _FMT(x) _Generic(x, int: "%d\n", char*: "%s\n")

int intcmp(int v1, int v2);

#define ASSERTION_ERROR                                                        \
    printf("\033[31;1m[ASSERTION ERROR]\033[0m " __FILE__ ":%d\n", __LINE__);

#define ASSERT_EQ(value, expected)                                             \
    {                                                                          \
        __auto_type v = value;                                                 \
        __auto_type e = expected;                                              \
        if (_Generic(v, int: intcmp, char*: strcmp)(v, e)) {                   \
            ASSERTION_ERROR;                                                   \
            printf("Value:    ");                                              \
            printf(_FMT(v), v);                                                \
            printf("Expected: ");                                              \
            printf(_FMT(e), e);                                                \
            return -1;                                                         \
        }                                                                      \
    }

void print_vec(int size, double vec[size]);

#define ASSERT_EQ_VEC(value, value_size, expected, expected_size)              \
    {                                                                          \
        double *v = value, *e = expected;                                      \
        int v_s = value_size, e_s = expected_size;                             \
        if (v_s != e_s || memcmp(v, e, sizeof(double) * e_s)) {                \
            ASSERTION_ERROR;                                                   \
            printf("Value:    ");                                              \
            print_vec(v_s, v);                                                 \
            printf("Expected: ");                                              \
            print_vec(e_s, e);                                                 \
            return -1;                                                         \
        }                                                                      \
    }

#endif // TESTS_H
