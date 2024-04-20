# Parameters

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = server/servidor_claves.c
SOURCES_SVC.h = 
SOURCES.x = claves_rpc.x

TARGETS_SVC.c = server/claves_rpc_svc.c server/claves_rpc_server.c server/claves_rpc_xdr.c 
TARGETS_CLNT.c = server/claves_rpc_clnt.c client/claves.c server/claves_rpc_xdr.c 
TARGETS = server/claves_rpc.h server/claves_rpc_xdr.c server/claves_rpc_clnt.c server/claves_rpc_svc.c client/claves.c server/claves_rpc_server.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)

# Compiler flags 

CPPFLAGS += -D_REENTRANT
CFLAGS += -Wall -Werror -Wno-unused-variable -fpic -g -I/usr/include/tirpc
LDLIBS += -lnsl -lpthread -ltirpc

SERVER=servidor
NAME_LIB=libclaves.so
LIB=-lclaves -L. -Wl,-rpath=.

# Targets 

.PHONY: all clean

all: $(NAME_LIB) cliente cliente_concurrent $(SERVER)

$(NAME_LIB): $(OBJECTS_CLNT)
	$(LINK.c) -o $(NAME_LIB) $(OBJECTS_CLNT) $(LDLIBS) --shared

cliente: client/cliente.o client/tests.o $(NAME_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

cliente_concurrent: client/cliente_concurrent.o client/tests.o $(NAME_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

clean:
	rm client/*.o server/*.o
