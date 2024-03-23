#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Converts a string to an int while checking for errors
 *
 * @param string Input string
 * @param destination Address in which the value will be stored
 * @return Error code of the conversion
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int string2int(const char* string, int* destination);

#endif // UTILS_H
