#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <string.h>

#include "../common/error_handling.h"

/**
 * @brief Generates a new test case function. Should be followed by its code
 * block
 *
 * @param name Name of the function
 */
#define TEST(name)                                                             \
    int test_##name();                                                         \
    void name() {                                                              \
        printf(test_##name() == 0 ? "\033[32;1m[SUCCESS]:"                     \
                                  : "\033[31;1m[FAILURE]:");                   \
        printf("\033[0m " #name "\n");                                         \
    }                                                                          \
    int test_##name()

/**
 * @brief Gets the appropriate format string to print an `int` or an `string`
 *
 * @return Correct format string
 */
#define _FMT(x) _Generic(x, int: "%d\n", char*: "%s\n")

/**
 * @brief Asserts that the given value has the expected value, and ends the test
 * if it doesn't. Only supports comparing `int`s and `string`s
 */
#define ASSERT_EQ(value, expected)                                             \
    {                                                                          \
        __auto_type _v = value;                                                \
        __auto_type _e = expected;                                             \
        if (_Generic(_v, int: intcmp, char*: strcmp)(_v, _e)) {                \
            printf(ERROR("ASSERTION_ERROR", "\n"));                            \
            printf("Input:    " #value "\n");                                  \
            printf("Value:    ");                                              \
            printf(_FMT(_v), _v);                                              \
            printf("Expected: ");                                              \
            printf(_FMT(_e), _e);                                              \
            return -1;                                                         \
        }                                                                      \
    }

/**
 * @brief Asserts that the given `double` vector has the expected size and
 * value, and ends the test if it doesn't
 */
#define ASSERT_EQ_VEC(value, value_size, expected, expected_size)              \
    {                                                                          \
        double *_v = value, *_e = expected;                                    \
        int _v_s = value_size, _e_s = expected_size;                           \
        if (_v_s != _e_s || memcmp(_v, _e, sizeof(double) * _e_s)) {           \
            printf(ERROR("ASSERTION_ERROR", "\n"));                            \
            printf("Input:    " #value "\n");                                  \
            printf("Value:    ");                                              \
            print_vec(_v_s, _v);                                               \
            printf("Expected: ");                                              \
            print_vec(_e_s, _e);                                               \
            return -1;                                                         \
        }                                                                      \
    }

/**
 * @brief Checks if 2 integers are different. Equivalent to `v1 != v2`, but
 * implemented as a function
 *
 * @param v1 First integer
 * @param v2 Second integer
 * @return Result of the comparison
 * @retval 0 The numbers were equal
 * @retval 1 The numbers were different
 */
int intcmp(int v1, int v2);

/**
 * @brief Prints a vector to `stdout`
 *
 * @param size Size of the vector
 * @param vec Vector to print
 */
void print_vec(int size, double vec[size]);

#endif // TESTS_H
