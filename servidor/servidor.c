#include <fcntl.h>
#include <pthread.h>
#include "linked_list.h"
#include <mqueue.h>
#include <stdbool.h>
#include <string.h>
#include "petitions.h"
#include <stdio.h>

#define MAXPETITIONS 10
linked_list *list;

pthread_mutex_t m_copy;
bool copied = false;
pthread_cond_t c_copy;

void process_petition(void *msg){
  
  petition p;
  response r;
  
  // zona de copia de mensajes.
  pthread_mutex_lock(&m_copy);
  
  p.op_id = ((petition *)msg)->op_id; 
  p.key = ((petition *)msg)->key;
  strcpy(p.val1,((petition *)msg)->val1);
  p.n_value2 = ((petition *)msg)->n_value2;

  for (int i = 0; i < p.n_value2; i++) {
    p.value2[i] = ((petition *)msg)->value2[i];
  }
  
  copied = true;
  pthread_cond_signal(&c_copy);
  pthread_mutex_unlock(&m_copy);

  int mq_resp = mq_open("/mq_resp", O_WRONLY);
  int resp = 0;
  

  // no es zona crítica ya que las operaciones se realizan de forma atómica
  // debido al mutex incorporado en la lista enlazada.

  switch (p.op_id) {
    
    // icialización de servicio 
    case 1:
      r.op_id = 1;
      clear(list);
      resp = 0;
      break;

    // creación de vector nuevo
    case 2:
      r.op_id = 2;
      resp = set_value(list, p.key, p.val1, p.n_value2, p.value2);
      break;

    // obtención de un vector ya creado.
    case 3:
      r.op_id = 3;
      resp = get_value(list, p.key, r.val1, &r.n_value2, r.value2);
      break;

    // modificación de un vector ya creado.
    case 4:
      r.op_id = 4;
      resp = modify_value(list, p.key, p.val1, p.n_value2, p.value2);
      break;
    
    // borrado de un vector con una clave específica.
    case 5:
      r.op_id = 5;
      resp = delete_key(list, p.key);
      break;

    // comprobación de existencia de una clave.
    case 6:
      r.op_id = 6;
      resp = exist(list, p.key);
      break;

    // caso de código de operación no válido.
    default:
      r.op_id = -1;
      resp = -1;
      break;
      
  }

  r.res_op = resp;

  // poner la respuesta en la cola correspondiente.
  mq_send(mq_resp, (char *)&r, sizeof(response),NULL);

  pthread_exit(0);
}


int main(int argc, char const *argv[])
{

  //inicializar mutex de copia y condition.
  pthread_mutex_init(&m_copy, NULL);
  pthread_cond_init(&c_copy, NULL);

  // inicializar las estructuras de datos para las colas y gestión de peticiones.
  list = init_list();

  pthread_t thrd_worker;
  
  pthread_attr_t *atts;
  pthread_attr_init(atts);
  pthread_attr_setdetachstate(atts, PTHREAD_CREATE_DETACHED);
  
  struct mq_attr attrs = { NULL, 10, sizeof(petition), NULL};
  int mq_pet = mq_open("/mq_pet", O_RDONLY | O_CREAT, 0777);
  
  // bucle de peticiones
  int i = 0;
  char msg[sizeof(petition)];

  while (1){

    mq_receive(mq_pet,msg,sizeof(petition),NULL);
    
    // soporte para operación de parar el servidor con código de operación 7.
    if (((petition *)msg)->op_id == 7){
      break;
    }

    pthread_create(&thrd_worker, atts , (void *)process_petition, (void *)msg);
    
    i += (1 % MAXPETITIONS);
    
    // esperar hasta que el hilo se haya copiado los argumentos
    // para crear otro.

    pthread_mutex_lock(&m_copy);
    while (copied != true) {
      pthread_cond_wait(&c_copy, &m_copy);
    }
    copied = false;
    pthread_mutex_unlock(&m_copy);
  }


  // cerrar las estructuras de datos.
  destroy(list);
  mq_close(mq_pet);
  pthread_attr_destroy(atts);

  //destruir todos los mutex y conditions.
  pthread_mutex_destroy(&m_copy);
  pthread_cond_destroy(&c_copy);
  return 0;
}
