CC=gcc
CFLAGS= -Wall -g
NAME_LIB= libclaves.so
LIB= -lclaves -L.

.PHONY: ALL

ALL: GEN_LIB BLD_CLI BLD_SER

GEN_LIB: cliente/claves.c
	$(CC) $(CFLAGS) -shared -o $(NAME_LIB) cliente/claves.c

BLD_CLI: $(NAME_LIB) cliente/cliente.c
	$(CC) $(CFLAGS) -o Cliente cliente/cliente.c $(LIB)
	echo "Instalando librería dinámica en el sistema:"

	sudo mv $(NAME_LIB) /usr/local/lib/$(NAME_LIB)
	sudo ldconfig

BLD_SER: servidor/linked_list.c servidor/servidor.c 
	$(CC) $(CFLAGS) -c servidor/linked_list.c -o servidor/l-list.o -lpthread
	$(CC) $(CFLAGS) -c servidor/servidor.c -o servidor/server.o -lpthread
	$(CC) $(CFLAGS) -o Servidor servidor/l-list.o servidor/server.o -lpthread -lgcc
	
