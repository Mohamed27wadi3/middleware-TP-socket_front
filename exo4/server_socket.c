#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5000
#define BACKLOG 5
#define BUFSZ 256

static void handle_client(int clientfd)
{
    char buf[BUFSZ];
    ssize_t n = recv(clientfd, buf, sizeof(buf) - 1, 0);
    if (n <= 0)
        return;
    buf[n] = '\0';

    int a = 0, b = 0;
    if (sscanf(buf, "%d %d", &a, &b) != 2) {
        const char *err = "ERROR bad input\n";
        send(clientfd, err, strlen(err), 0);
        return;
    }

    int sum = a + b;
    int diff = a - b;
    int prod = a * b;
    int div_zero = (b == 0);
    double quot = div_zero ? 0.0 : (double)a / (double)b;

    char out[BUFSZ];
    int len = snprintf(out, sizeof(out),
                       "SUM:%d\nDIFF:%d\nPROD:%d\nDIV_ZERO:%d\nQUOT:%.3f\n",
                       sum, diff, prod, div_zero, quot);
    send(clientfd, out, len, 0);
}

int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, BACKLOG) < 0) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    printf("[socket-server] en écoute sur le port %d\n", PORT);

    while (1) {
        int clientfd = accept(sockfd, NULL, NULL);
        if (clientfd < 0) {
            perror("accept");
            continue;
        }
        handle_client(clientfd);
        close(clientfd);
    }

    close(sockfd);
    return 0;
}
