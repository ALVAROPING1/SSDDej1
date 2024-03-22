#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "./error_handling.h"
#include "./env.h"

static int createRegex(regex_t *regex,const char *re){
  int res = regcomp( regex, re,0);
  if (res == 0) {return 0;} else {return -1;}
}

static int matchRegex(const char *reg_pattern, char *str){
  regex_t reg;
  CHECK(createRegex(&reg,reg_pattern),"REGEX CANNOT BE CREATED",-1);

  return regexec( &reg, str,0,NULL,0);
}

int getServerIp(char *ip,int *match){
  char *var = getenv("IP_TUPLAS");
  CHECK(var,"SERVER IP NOT DEFINED!!\n",NULL);
  strcpy(ip, var);
  *match = matchRegex("[:number:]*.[:number:]*.[:number:]*.[:number:]*",var);
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

