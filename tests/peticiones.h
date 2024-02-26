#ifndef PETICIONES_H
#define PETICIONES_H 1

typedef struct peticion {
  char msg[256];
  int num;
} peticion;

typedef struct response {
  char resp[256];
  int res_op;
} response;

#endif // !PETICIONES_H
