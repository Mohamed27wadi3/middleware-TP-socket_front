#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>

#include "increment.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <serveur> [valeur]\n", argv[0]);
        return 1;
    }

    int val = 0;
    if (argc >= 3) {
        val = atoi(argv[2]);
    } else {
        printf("Entrer un nombre : ");
        if (scanf("%d", &val) != 1) {
            fprintf(stderr, "Entrée invalide.\n");
            return 1;
        }
    }

    CLIENT *clnt = clnt_create(argv[1], INCRPROG, INCRVERS, "tcp");
    if (!clnt) {
        clnt_pcreateerror(argv[1]);
        return 1;
    }

    int *res = increment_1(val, clnt);
    if (!res) {
        clnt_perror(clnt, "increment");
        clnt_destroy(clnt);
        return 1;
    }

    printf("Resultat = %d\n", *res);
    clnt_destroy(clnt);
    return 0;
}
