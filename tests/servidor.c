#include <string.h>
#include <stdio.h>
#include <mqueue.h>
#include "peticiones.h"


int main(int argc, char *argv[])
{ 
  struct mq_attr atts;
  memset(&atts, 0, sizeof(struct mq_attr));

  atts.mq_maxmsg = 10;
  atts.mq_msgsize = sizeof(peticion);

  int mq_pet = mq_open("/mq_pet", O_CREAT | O_RDONLY, 0777, &atts);
  int mq_resp = mq_open("/mq_resp", O_WRONLY);

  peticion p;
  response r;

  memset(&p,0, sizeof(peticion));
  memset(&r,0, sizeof(response));

  mq_receive(mq_pet, (char *)&p, sizeof(peticion), NULL);
  r.res_op = 0;
  strcpy(r.resp, "Respuesta generada");
  mq_send(mq_resp, (char *)&r, sizeof(response), 0);

  printf("Atendimiento correcto!!");

  mq_close(mq_pet);
  mq_close(mq_resp);

  return 0;
}
