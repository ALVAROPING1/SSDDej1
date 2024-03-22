#ifndef ENV_H
#define ENV_H 1

  /*
   * @brief Function that takes an string storage and stores the content of the
   * environment variable in it.
   *
   * @param ip_store The space where the content of the environment variable "IP_TUPLAS" will be stored.
   * @param res_match The place where the values 0 (is a match) and REG_NOMATCH (not a match) are stored.
   *
   * @retval 0 Returns '0' if the variable is defined and can be obtained from the environment.
   * @retval -1 Return '-1' when the variable isn't defined or it cannot be read.
   *
   */
  int getServerIp(char *ip_store,int *res_match);

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

  /*
   * @brief Function that receives a pointer where the regex is stored and creates it.
   * 
   * @param regex The space were the regex will be created.
   * @param re Pattern of recognission for the regex.
   * 
   * @retval 0 Returns '0' where the regex is created successfully.
   * @retval -1 Returns '-1' if the regex couldn't be created. 
   */
  static int createRegex(regex_t *regex, const char *re);

  /*
   * @brief Function that, given a regex pattern for recognission and an string, creates the regex and
   * checks if there is a match whith the string.
   * 
   * @param reg_pattern Recognission pattern for creating the regexp.
   * @param str String to do the match on with the regexp.
   * 
   * @retval 0 Retruns this value if there is a match on the given string.
   * @retval REG_NOMATCH Returns that value if there isn't a match.
   */
  static int matchRegex(const char *reg_pattern, char *str);

#endif // !ENV_H

