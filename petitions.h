#ifndef PETITION_H
#define PETITION_H 1

#define MAXVAL1 256
#define MAXVAL2 32

typedef struct petition {
  int op_id;
  int key;
  char val1[MAXVAL1];
  int n_value2;
  double value2[MAXVAL2];
} petition;

typedef struct response {
  int op_id;
  char val1[MAXVAL1];
  int n_value2;
  double value2[MAXVAL2];
  int res_op;
} response;

void init_petition(petition p){
  p.op_id = 0;
  p.key = 0;
  for (int i = 0; i < MAXVAL1; i++) {
    p.val1[i] = 0;
  }
  p.n_value2 = 0;
  for (int i = 0; i < MAXVAL2; i++) {
    p.value2[i] = 0;
  }
}

void init_response(response r){
  r.op_id = 0;
  for (int i = 0; i < MAXVAL1; i++) {
    r.val1[i] = 0;
  }
  r.n_value2 = 0;
  for (int i = 0; i < MAXVAL2; i++) {
    r.value2[i] = 0;
  }
}

#endif // !PETITION_H
