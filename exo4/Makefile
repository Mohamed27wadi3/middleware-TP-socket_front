CC = gcc -ansi -D_GNU_SOURCE
CFLAGS = -g
PROGS = socket_server socket_client

all: $(PROGS)

socket_server: server_socket.c
	$(CC) $(CFLAGS) -o $@ server_socket.c

socket_client: client_socket.c
	$(CC) $(CFLAGS) -o $@ client_socket.c

clean:
	rm -f $(PROGS) *.o

.PHONY: all clean
