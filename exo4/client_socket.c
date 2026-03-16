#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5000
#define BUFSZ 256

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <serveur> <a> <b>\n", argv[0]);
        return 1;
    }

    const char *host = argv[1];
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    char payload[BUFSZ];
    int len = snprintf(payload, sizeof(payload), "%d %d\n", a, b);
    send(sockfd, payload, len, 0);

    char resp[BUFSZ];
    ssize_t n = recv(sockfd, resp, sizeof(resp) - 1, 0);
    if (n > 0) {
        resp[n] = '\0';
        printf("Réponse serveur:\n%s", resp);
    } else {
        printf("Aucune réponse.\n");
    }

    close(sockfd);
    return 0;
}
