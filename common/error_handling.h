#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>

/**
 * Formats an error message
 * @param msg Error message
 * @return Formatted message
 */
#define ERROR(msg) "\033[31;1m[ERROR]:\033[0m " msg "\n"

/**
 * `CHECK()` macro with 3 parameters
 */
#define _CHECK3(expr, err_msg, err_val)                                        \
    ({                                                                         \
        __auto_type res = expr;                                                \
        if (res == err_val) {                                                  \
            perror(err_msg);                                                   \
            return -1;                                                         \
        }                                                                      \
        res;                                                                   \
    })

/**
 * `CHECK()` macro with 2 parameters
 */
#define _CHECK2(expr, err_msg)                                                 \
    if ((expr) != 0) {                                                         \
        perror(err_msg);                                                       \
        return -1;                                                             \
    }

/**
 * `CHECK()` macro with a single parameter
 */
#define _CHECK1(expr)                                                          \
    if ((expr) != 0) { return -1; }

// Gets the appropriate macro for the amount of arguments
#define _GET_CHECK(_1, _2, _3, NAME, ...) NAME

/**
 * Checks that the given expression didn't result in an error. Evaluates to its
 * value if there wasn't an error, and returns from the current function with -1
 * otherwise
 *
 * @param expr Expression to evaluate
 * @param err_msg [Optional] Error message
 * @param err_val [Optional] Error value to check for. If unspecified, any non 0
 *      value is considered an error
 * @param ... Format values
 */
#define CHECK(...)                                                             \
    _GET_CHECK(__VA_ARGS__, _CHECK3, _CHECK2, _CHECK1)(__VA_ARGS__)

/**
 * Asserts that a given expression is `true`, printing a formatted message to
 * `stderr` and returning from the current function otherwise
 *
 * @param expr Expression to evaluate
 * @param err_msg Error message format string
 * @param ... Format values
 */
#define ASSERT(expr, err_msg, ...)                                             \
    if (!(expr)) {                                                             \
        fprintf(stderr, ERROR(err_msg), ##__VA_ARGS__);                        \
        return -1;                                                             \
    }

#endif // ERROR_HANDLING_H
