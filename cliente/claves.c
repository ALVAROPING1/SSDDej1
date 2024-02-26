#include "../servidor/petitions.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_service(){
  struct mq_attr attr = {
    0777,
    10,
    sizeof(petition),
    0
  };
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDWR, 0777,&attr);

  petition p;
  response r;
  memset(&p,0,sizeof(petition));
  memset(&r,0,sizeof(response));

  p.op_id = 1;

  mq_send(mq_pet, (char *)&p, sizeof(petition), 0);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  mq_close(mq_pet);
  mq_close(mq_resp);

  return r.res_op;
}

int set_value(int key, char *value1, int n_value2, double *value2){
  struct mq_attr attr = {
    0777,
    10,
    sizeof(petition),
    0
  };
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDWR, 0777,&attr);

  petition p;
  response r;
  memset(&p,0,sizeof(petition));
  memset(&r,0,sizeof(response));

  
  p.op_id = 2;
  p.key = key;
  strcpy(p.val1, value1);
  p.n_value2 = n_value2;
  for (int i = 0; i < p.n_value2; i++) {
    p.value2[i] = value2[i];
  }

  mq_send(mq_pet, (char *)&p, sizeof(petition),0);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);
  
  mq_close(mq_pet);
  mq_close(mq_resp);
  return r.res_op;

} 

int get_value(int key, char *value1, int *n_value, double *value2){
  
  struct mq_attr attr = {
    0777,
    10,
    sizeof(petition),
    0
  };
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDWR, 0777,&attr);

  petition p;
  response r;
  memset(&p,0,sizeof(petition));
  memset(&r,0,sizeof(response));
  

  p.op_id = 3;
  p.key = key;

  mq_send(mq_pet, (char *)&p, sizeof(petition),0);
  mq_receive(mq_resp, (char *)&r, sizeof(response),NULL);

  if (r.res_op == 0){
    strcpy(value1, r.val1);
    *n_value = r.n_value2;
    for (int i = 0; i < r.n_value2; i++) {
      value2[i] = r.value2[i];
    }
    mq_close(mq_pet);
    mq_close(mq_resp); 
    return 0;
  }

  mq_close(mq_pet);
  mq_close(mq_resp); 
  return -1;
}

int modify_value(int key, char *value1, int n_value2, double *value2){
  
  struct mq_attr attr = {
    0777,
    10,
    sizeof(petition),
    0
  };
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDWR, 0777,&attr);

  petition p;
  response r;
  memset(&p,0,sizeof(petition));
  memset(&r,0,sizeof(response));

  
  p.op_id = 4;
  p.key = key;
  strcpy(p.val1, value1);
  p.n_value2 = n_value2;
  for (int i = 0; i < p.n_value2; i++) {
    p.value2[i] = value2[i];
  }

  mq_send(mq_pet, (char *)&p, sizeof(petition),0);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);
  

  mq_close(mq_pet);
  mq_close(mq_resp);
  return r.res_op;

}

int delete_key(int key){
  
  struct mq_attr attr = {
    0777,
    10,
    sizeof(petition),
    0
  };
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDWR, 0777,&attr);

  petition p;
  response r;
  memset(&p,0,sizeof(petition));
  memset(&r,0,sizeof(response));


  p.op_id = 5;
  p.key = key;
  
  mq_send(mq_pet, (char *)&p, sizeof(petition),0);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  mq_close(mq_pet);
  mq_close(mq_resp);
  return r.res_op;
}

int exist(int key){
  
  struct mq_attr attr = {
    0777,
    10,
    sizeof(petition),
    0
  };
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("/mq_resp", O_CREAT | O_RDWR, 0777,&attr);

  petition p;
  response r;
  memset(&p,0,sizeof(petition));
  memset(&r,0,sizeof(response));


  p.op_id = 6;
  p.key = key;
  
  mq_send(mq_pet, (char *)&p, sizeof(petition),0);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);
  
  mq_close(mq_pet);
  mq_close(mq_resp);
  return r.res_op;

}

int exit_service(){
 
  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  
  petition p;
  memset(&p,0,sizeof(petition));
  
  p.op_id = 7;

  mq_send(mq_pet, (char *)&p, sizeof(petition), 0);
  
  mq_close(mq_pet);
  return 0;
}
