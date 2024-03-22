#ifndef ENV_H
#define ENV_H 1

  /*
   * @brief Function that takes an string storage and stores the content of the
   * environment variable in it.
   *
   * @param ip_store The space where the content of the environment variable "IP_TUPLAS" will be stored.
   *
   * @retval 0 Returns '0' if the variable is defined and can be obtained from the environment.
   * @retval -1 Return '-1' when the variable isn't defined or it cannot be read.
   *
   */
  int getServerIp(char *ip_store);

  /*
   * @brief Funtion that, given an int allocated storage, it stores the 
   * port defined by the environment variable "PORT_TUPLAS".
   *
   * @param port_store The space where the content of the environment variable "PORT_TUPLAS" will be stored.
   *
   * @retval -1 Returns '-1' if the environment doesn't contain the "PORT_TUPLAS" variable or it cannot be
   * converted to an integer number.
   *
   * @retval 0 Returns '0' if the variable is substracted of the environment and succesfully converted to an
   * integer number.
   *
   */
  int getServerPort(int *port_store);

#endif // !ENV_H

