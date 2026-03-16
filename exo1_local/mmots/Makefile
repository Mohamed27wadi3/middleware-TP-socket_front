# rpcgen example Makefile
CC = gcc -ansi -D_GNU_SOURCE
PROGS = mmots mmotsd

DEFS =
RPC_CFLAGS = $(shell pkg-config --cflags libtirpc 2>/dev/null)
RPC_LIBS   = $(shell pkg-config --libs libtirpc 2>/dev/null || echo "-ltirpc")
CFLAGS = -g ${DEFS} ${INCLUDE} ${RPC_CFLAGS}
LDLIBS = ${RPC_LIBS}

SRC = mmots.c mmots.x mmots_cif.c mmots_server.c mmots_srp.c
GFILES = mmots.h mmots_clnt.c mmots_svc.c mmots_xdr.c

MMOTS_OBJ = mmots_clnt.o mmots_cif.o mmots.o
MMOTSD_OBJ = mmots_svc.o mmots_server.o mmots_srp.o

all: ${PROGS}

mmots: ${MMOTS_OBJ} mmots_xdr.o
	${CC} ${CFLAGS} -o $@ ${MMOTS_OBJ} mmots_xdr.o ${LDLIBS}

mmotsd: ${MMOTSD_OBJ} mmots_xdr.o
	${CC} ${CFLAGS} -o $@ ${MMOTSD_OBJ} mmots_xdr.o ${LDLIBS}

mmots.h mmots_clnt.c mmots_svc.c mmots_xdr.c: mmots.x
	rpcgen -C -N mmots.x

clean: FRC
	rm -f Makefile.bak a.out tags core make.out ${PROGS} ${GFILES} *.o

install: all FRC
	@echo nothing to install.

print: Makefile ${SRC} FRC
	lpr -J"${PROG} source" Makefile ${SRC}

tags: ${SRC}
	ctags ${SRC}

FRC:
	
mmots: mmots.c ${MMOTS_OBJ} ${GFILES} mmots.x
mmotsd: ${MMOTSD_OBJ} ${GFILES} mmots.x
