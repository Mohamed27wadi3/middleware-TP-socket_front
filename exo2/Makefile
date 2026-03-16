CC = gcc -ansi -D_GNU_SOURCE
RPCGEN = rpcgen
RPC_FLAGS = -C -N

RPC_CFLAGS = $(shell pkg-config --cflags libtirpc 2>/dev/null)
RPC_LIBS   = $(shell pkg-config --libs libtirpc 2>/dev/null || echo "-ltirpc")
CFLAGS = -g ${DEFS} ${INCLUDE} ${RPC_CFLAGS}
LDLIBS = ${RPC_LIBS}

GFILES = increment.h increment_clnt.c increment_svc.c
PROGS = increment_client increment_server

all: $(PROGS)

$(GFILES): increment.x
	$(RPCGEN) $(RPC_FLAGS) increment.x

increment_client.o increment_server.o increment_clnt.o increment_svc.o: $(GFILES)

increment_client: increment_client.o increment_clnt.o
	$(CC) $(CFLAGS) -o $@ increment_client.o increment_clnt.o $(LDLIBS)

increment_server: increment_server.o increment_svc.o
	$(CC) $(CFLAGS) -o $@ increment_server.o increment_svc.o $(LDLIBS)
	@ln -sf increment_client incr_client
	@ln -sf increment_server incr_server

clean:
	rm -f $(PROGS) incr_client incr_server $(GFILES) *.o

.PHONY: all clean
