CC = gcc -ansi -D_GNU_SOURCE
RPCGEN = rpcgen
RPC_FLAGS = -C -N

RPC_CFLAGS = $(shell pkg-config --cflags libtirpc 2>/dev/null)
RPC_LIBS   = $(shell pkg-config --libs libtirpc 2>/dev/null || echo "-ltirpc")
CFLAGS = -g ${DEFS} ${INCLUDE} ${RPC_CFLAGS}
LDLIBS = ${RPC_LIBS}

GFILES = calc.h calc_clnt.c calc_svc.c calc_xdr.c
PROGS = calc_client calc_server

all: $(PROGS)

$(GFILES): calc.x
	$(RPCGEN) $(RPC_FLAGS) calc.x

calc_client.o calc_server.o calc_clnt.o calc_svc.o calc_xdr.o: $(GFILES)

calc_client: calc_client.o calc_clnt.o calc_xdr.o
	$(CC) $(CFLAGS) -o $@ calc_client.o calc_clnt.o calc_xdr.o $(LDLIBS)

calc_server: calc_server.o calc_svc.o calc_xdr.o
	$(CC) $(CFLAGS) -o $@ calc_server.o calc_svc.o calc_xdr.o $(LDLIBS)

clean:
	rm -f $(PROGS) $(GFILES) *.o

.PHONY: all clean
