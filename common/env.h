#ifndef ENV_H
#define ENV_H

/**
 * @brief Gets the server IP/name as a string from the `IP_TUPLAS` environment
 * vaariable
 *
 * @param ip_store The space where the result will be stored
 * @return Error code of the operation
 * @retval 0 Successful execution
 * @retval -1 There was an error in the execution
 */
int getServerIp(char* ip_store);

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

#endif // ENV_H
