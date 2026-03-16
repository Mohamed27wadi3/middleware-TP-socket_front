#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <serveur> <a> <b>\n", argv[0]);
        return 1;
    }

    operands op;
    op.a = atoi(argv[2]);
    op.b = atoi(argv[3]);

    CLIENT *clnt = clnt_create(argv[1], CALCPROG, CALCVERS, "tcp");
    if (!clnt) {
        clnt_pcreateerror(argv[1]);
        return 1;
    }

    op_result *res = compute_1(op, clnt);
    if (!res) {
        clnt_perror(clnt, "compute");
        clnt_destroy(clnt);
        return 1;
    }

    printf("a=%d, b=%d\n", op.a, op.b);
    printf("a+b = %d\n", res->sum);
    printf("a-b = %d\n", res->diff);
    printf("a*b = %d\n", res->prod);
    if (res->div_zero)
        printf("a/b = division par zero\n");
    else
        printf("a/b = %.3f\n", res->quot);

    clnt_destroy(clnt);
    return 0;
}
