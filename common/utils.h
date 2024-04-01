#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Converts a string to an int while checking for errors
 *
 * @param string Input string
 * @param destination Address in which the value will be stored
 * @return Error code of the conversion
 * @retval 0 Successfull execution
 * @retval -1 There was an error in the execution
 */
int string2int(const char* string, int* destination);


/**
 * @brief Converts a vector of doubles to a serialized string.
 *
 * @param str_m Position of memory where the serialized vector
 * will be stored.
 *
 * @param len length of the vector to be serialized.
 *
 * @param v_m The actual position of the vector that will be serialized.
 *
 * @retval 0 Successfull execution
 * @retval -1 Error value.
 */
int serializeVector(char *str_m, int len, double *v_m);

/**
 * @brief Given a string, it extracts the vector components
 * to a pre-allocated memory buffer.
 *
 * @param stream Position of memory where the serialized vector
 * is located.
 *
 * @param len length of the serialized vector.
 *
 * @param v_m The position for the vector to ve stored.
 *
 * @retval 0 Successfull execution
 * @retval -1 Error value.
 */
int deserializeVector(char *stream, int len, double **v_m);


#endif // UTILS_H
