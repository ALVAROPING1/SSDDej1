#include <string.h>
#include <stdio.h>
#include "claves.h"

int main(int argc, char *argv[])
{ 
  init();
  
  char pal[256];
  int n_v;
  double vec_nums[32];
  int res = get_value(0, pal, &n_v, vec_nums);
  if (res == -1){
    printf("Test Get_Value con servidor vacío de elementos: True.\n");
  } else {
    printf("Test Get_Value con servidor vacío de elementos: False.\n");
  }

  char str[] = "hola!";
  double vec[3] = {1,2,3};
  int res2 = set_value(1, str, 3, vec);

  if (exist(1) == 1 && res2 == 0){
    printf("Test de inserción de un vector en servidor: True.\n");
  } else {
    printf("Test de inserción de un vector en servidor: False.\n");
  }

  char cad[256];
  int n_value2;
  double vec2[32];
  int res3 = get_value(1, cad, &n_value2, vec2);

  if (
      res3 != -1 &&
      strcmp(cad, str) == 0 &&
      n_value2 == 3 &&
      vec2[0] == vec[0] &&
      vec2[1] == vec[1] &&
      vec2[2] == vec[2]
    ){
    printf("Test de obtención de un vector existente: True.\n");
  }
  else printf("Test de obtención de un vector existente: False.\n");

  int res4 = exist(1);
  if (res4 == 1){
    printf("Test de existencia sobre elemento existente: True.\n");
  } else {
    printf("Test de existencia sobre elemento existente: False.\n");
  }

  int res5 = delete_key(1);
  
  if (res5 == 0 && exist(1) == 0){
    printf("Test de eliminación y existencia de un elemento existente: True.\n");
  } else {
    printf("Test de eliminación y existencia de un elemento existente: False.\n");
  }



  return 0;
}
