#include <string.h>
#include <stdio.h>
#include <mqueue.h>
#include "peticiones.h"


int main(int argc, char *argv[])
{ 
  struct mq_attr atts;
  memset(&atts, 0, sizeof(struct mq_attr));

  int mq_pet = mq_open("/mq_pet", O_WRONLY);
  int mq_resp = mq_open("mq_resp",O_CREAT | O_RDONLY, 0777, &atts);

  peticion p;
  response r;
  memset(&p,0, sizeof(peticion));
  memset(&r,0, sizeof(response));

  mq_send(mq_pet, (char *)&p, sizeof(peticion), 0);
  mq_receive(mq_resp, (char *)&r, sizeof(response), NULL);

  if (r.res_op == 0 && strcmp(r.resp, "Respuesta generada")){
    printf("Operación realizada con éxito!!\n");
  } else {
    printf("Operación fracasada!!");
  }

  mq_close(mq_pet);
  mq_close(mq_resp);

  return 0;
}
