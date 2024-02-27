CC=gcc
CFLAGS=-Wall -Werror -g
NAME_LIB=libclaves.so
LIB=-lclaves -L. -Wl,-rpath=.

.PHONY: all clean

all: $(NAME_LIB) cliente servidor

$(NAME_LIB): client/claves.c
	$(CC) $(CFLAGS) -shared -o $(NAME_LIB) -fpic client/claves.c

client/claves.o:
	:

%.o: %.c
	@echo "Compiling... $<"
	$(CC) $(CFLAGS) -c $< -o $@

cliente: client/cliente.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

servidor: server/linked_list.o server/servidor_handler.o server/servidor.o
	$(CC) $(CFLAGS) -lpthread -o $@ $^

clean:
	rm client/*.o
	rm server/*.o
