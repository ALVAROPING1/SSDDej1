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


#endif // !PETITION_H
