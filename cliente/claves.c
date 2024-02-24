#include "../servidor/petitions.h"
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

int init(){
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDONLY, 0777);
  
  petition p = {1,NULL,NULL,NULL,NULL};
  response r;

  mq_send(mq_pet, (char *)&p, sizeof(petition), NULL);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  return r.res_op;
}

int set_value(int key, char *value1, int n_value2, double *value2){
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDONLY, 0777);

  petition p;
  response r;
  
  p.op_id = 2;
  p.key = key;
  strcpy(p.val1, value1);
  p.n_value2 = n_value2;
  for (int i = 0; i < p.n_value2; i++) {
    p.value2[i] = value2[i];
  }

  mq_send(mq_pet, (char *)&p, sizeof(petition), NULL);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  return r.res_op;

} 

int get_value(int key, char *value1, int *n_value, double *value2){
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDONLY, 0777);

  petition p = {3, key,NULL,NULL,NULL};
  response r;

  mq_send(mq_pet, (char *)&p, sizeof(petition), NULL);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  if (r.res_op == 0){
    strcpy(value1, r.val1);
    *n_value = r.n_value2;
    for (int i = 0; i < r.n_value2; i++) {
      value2[i] = r.value2[i];
    }
    return 0;
  }

  return -1;
}

int modify_value(int key, char *value1, int n_value2, double *value2){
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDONLY, 0777);

  petition p;
  response r;
  
  p.op_id = 4;
  p.key = key;
  strcpy(p.val1, value1);
  p.n_value2 = n_value2;
  for (int i = 0; i < p.n_value2; i++) {
    p.value2[i] = value2[i];
  }

  mq_send(mq_pet, (char *)&p, sizeof(petition), NULL);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  return r.res_op;

}

int delete_key(int key){
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDONLY, 0777);

  petition p;
  response r;
  
  p.op_id = 5;
  p.key = key;
  
  mq_send(mq_pet, (char *)&p, sizeof(petition), NULL);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  return r.res_op;

}

int exist(int key){
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDONLY, 0777);

  petition p;
  response r;
  
  p.op_id = 6;
  p.key = key;
  
  mq_send(mq_pet, (char *)&p, sizeof(petition), NULL);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  return r.res_op;

}
