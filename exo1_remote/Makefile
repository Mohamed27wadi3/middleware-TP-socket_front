CC = gcc -ansi -D_GNU_SOURCE
RPCGEN = rpcgen
RPC_FLAGS = -C -N
RPC_SRC = common/mmots.x
RPC_GEN = common/mmots.h common/mmots_clnt.c common/mmots_svc.c common/mmots_xdr.c

RPC_CFLAGS = $(shell pkg-config --cflags libtirpc 2>/dev/null)
RPC_LIBS   = $(shell pkg-config --libs libtirpc 2>/dev/null || echo "-ltirpc")
CFLAGS = -g ${DEFS} ${INCLUDE} ${RPC_CFLAGS}
LDLIBS = ${RPC_LIBS}

CLIENT_BIN = client/mmots_remote
SERVER_BIN = server/mmotsd_remote

CLIENT_OBJ = client/main.o common/mmots_clnt.o common/mmots_xdr.o
SERVER_OBJ = server/logic.o server/handlers.o common/mmots_svc.o common/mmots_xdr.o

all: $(CLIENT_BIN) $(SERVER_BIN)

$(RPC_GEN): $(RPC_SRC)
	cd common && $(RPCGEN) $(RPC_FLAGS) mmots.x

common/mmots_clnt.o common/mmots_svc.o common/mmots_xdr.o: $(RPC_GEN)

$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJ) $(LDLIBS)

$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJ) $(LDLIBS)

client/main.o: client/main.c common/mmots.h
server/logic.o server/handlers.o: server/*.c common/mmots.h

clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN) common/mmots.h common/mmots_clnt.c common/mmots_svc.c common/mmots_xdr.c
	rm -f client/*.o server/*.o common/*.o

.PHONY: all clean
