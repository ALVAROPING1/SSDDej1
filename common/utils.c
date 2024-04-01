#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "claves.h"
#include "error_handling.h"
#include "utils.h"

int string2int(const char* string, int* destination) {
  char* leftover;
  // Tries to convert the string to an int
  *destination = strtol(string, &leftover, 10);
  // If the entire string was consumed in the conversion,
  // the leftover string should be empty
  CHECK(leftover[0] != '\0');
  return 0;
}

int serializeVector(char *str_m, int len, double *v_m){
  char result_str[MAX_STR_SIZE] = "";
  char str_num[MAX_STR_SIZE] = "";
  int r;

  strncat(result_str, " {", 3);

  for (int i = 0 ; i < len; i++){
    int r1;
    if (i != len-1) { 
      r1 = snprintf(str_num,MAX_STR_SIZE, "%lf,",v_m[i]);
    }
    else {r1 = snprintf(str_num,MAX_STR_SIZE, "%lf",v_m[i]);}
    
    strcat(result_str,str_num);
    
    if (r == 0 && r1 < 0) r=1;
  }

  strncat(result_str, " }", 3);

  CHECK(r);
  return 0;
}

int deserializeVector(char *stream, int len, double *vec_m[]){
  char vec_comp[MAX_STR_SIZE] = "";
  
  CHECK(sscanf(stream, "{ %s }",vec_comp) == 0);

  double *vector = vec_m[0];

  char *token = strtok(vec_comp, ",");
  int i = 0;
  while (token != NULL){
    CHECK(sscanf(token, "%lf",vector + i) == 0);
    token = strtok(NULL, ",");
    i++;
  }

  return 0;
  
}
