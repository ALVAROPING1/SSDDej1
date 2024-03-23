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

/**
 * @brief Gets the server IP/name as a string from the `IP_TUPLAS` environment
 * variable
 *
 * @param ip_store The space where the resulting pointer will be stored
 * @return Error code of the operation
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int getServerIp(char** ip_store);

/**
 * @brief Gets the server port as an int from the `PORT_TUPLAS` environment
 * variable
 *
 * @param port_store The space where the result will be stored
 * @return Error code of the operation
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int getServerPort(int* port_store);

#endif // UTILS_H
