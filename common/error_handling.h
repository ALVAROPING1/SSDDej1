#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>

/**
 * @brief Wrapper for the `_STR()` macro. Required so that the input macro is
 * expanded before being converted to a string
 */
#define _STR_WRAPPER(x) #x
/**
 * @brief Converts a macro constant to a string
 */
#define _STR(x) _STR_WRAPPER(x)

/**
 * @brief Formats a warn message
 *
 * @param msg Error message
 * @return Formatted message
 */
#define WARN(msg)                                                              \
    "\033[33;1m[WARN]:\033[0m " __FILE__ ":" _STR(__LINE__) ": " msg

/**
 * @brief Formats an error message
 *
 * @param err Error type
 * @param msg Error message
 * @return Formatted message
 */
#define ERROR(err, msg)                                                        \
    "\033[31;1m[" err "]:\033[0m " __FILE__ ":" _STR(__LINE__) ": " msg

/**
 * @brief Formats an error message for the `CHECK()`/`ASSERT()` macros. Can be
 * overridden to customize the format. If overridden, it should expand to the
 * `WARN()`/`ERROR()` macros
 */
#define FMT_ERROR(msg) ERROR("ERROR", msg)

/**
 * @brief `CHECK()` macro with 3 parameters
 */
#define _CHECK3(expr, err_msg, err_val)                                        \
    ({                                                                         \
        typeof(expr) _res = expr;                                              \
        if (_res == err_val) {                                                 \
            perror(FMT_ERROR(err_msg));                                        \
            return -1;                                                         \
        }                                                                      \
        _res;                                                                  \
    })

/**
 * @brief `CHECK()` macro with 2 parameters
 */
#define _CHECK2(expr, err_msg)                                                 \
    if ((expr) != 0) {                                                         \
        perror(FMT_ERROR(err_msg));                                            \
        return -1;                                                             \
    }

/**
 * @brief `CHECK()` macro with a single parameter
 */
#define _CHECK1(expr)                                                          \
    if ((expr) != 0) { return -1; }

// Gets the appropriate macro for the amount of arguments
#define _GET_CHECK(_1, _2, _3, NAME, ...) NAME

/**
 * @brief Checks that the given expression didn't result in an error. Evaluates
 * to its value if there wasn't an error, and returns -1 from the current
 * function otherwise. The `FMT_ERROR()` macro can be overridden to customize
 * the error message format
 *
 * @param expr Expression to evaluate
 * @param err_msg [Optional] Error message
 * @param err_val [Optional] Error value to check for. If unspecified, any non 0
 *      value is considered to be an error
 */
#define CHECK(...)                                                             \
    _GET_CHECK(__VA_ARGS__, _CHECK3, _CHECK2, _CHECK1)(__VA_ARGS__)

/**
 * @brief Asserts that a given expression is `true`, printing a formatted
 * message to `stderr` and returning -1 from the current function otherwise.
 * The `FMT_ERROR()` macro can be overridden to customize the error message
 * format
 *
 * @param expr Expression to evaluate
 * @param err_msg Error message format string
 * @param ... Format values
 */
#define ASSERT(expr, err_msg, ...)                                             \
    if (!(expr)) {                                                             \
        fprintf(stderr, FMT_ERROR(err_msg "\n"), ##__VA_ARGS__);               \
        return -1;                                                             \
    }

#endif // ERROR_HANDLING_H
