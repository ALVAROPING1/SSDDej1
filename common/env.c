#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./error_handling.h"
#include "./env.h"

int getServerIp(char *ip){
  char *var = getenv("IP_TUPLAS");
  CHECK(var,"SERVER IP NOT DEFINED!!\n",NULL);
  strcpy(ip, var);
  return 0;
}

int getServerPort(int *port){
  char *var = getenv("PORT_TUPLAS");
  CHECK(var,"SERVER PORT NOT DEFINED!!\n",NULL);
  int val = atoi(var);
  CHECK(val,"SERVER PORT NOT A NUMBER!!\n",0);
  *port = val;
  return 0;
}

