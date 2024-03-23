CC=gcc
CFLAGS=-Wall -Werror -g
NAME_LIB=libclaves.so
LIB=-lclaves -L. -Wl,-rpath=.

.PHONY: all clean

all: $(NAME_LIB) cliente cliente_concurrent servidor

$(NAME_LIB): client/claves.c common/comm.c common/utils.o
	$(CC) $(CFLAGS) -shared -o $(NAME_LIB) -fpic $^

%.o: %.c
	@echo "Compiling... $<"
	$(CC) $(CFLAGS) -c $< -o $@

cliente: client/cliente.o client/tests.o $(NAME_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

cliente_concurrent: client/cliente_concurrent.o client/tests.o $(NAME_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

servidor: server/servidor_claves.o server/servidor_handler.o server/servidor.o common/comm.c common/utils.o
	$(CC) $(CFLAGS) -lpthread -o $@ $^

clean:
	rm client/*.o
	rm server/*.o
