#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../common/mmots.h"

#define TAILLEMOT 50

static int read_command(char *cmd, char *word)
{
    printf("Commande: ");
    scanf("%s", cmd);
    if (*cmd != 'q' && *cmd != 'I' && *cmd != 'n') {
        printf("Mot: ");
        scanf("%50s", word);
        return (int)strlen(word);
    }
    return -1;
}

static int rpc_initialise(CLIENT *clnt)
{
    int *res = initialise_1(clnt);
    if (!res) {
        clnt_perror(clnt, "initialise");
        exit(1);
    }
    return *res;
}

static int rpc_insertion(CLIENT *clnt, char *word)
{
    int *res = insertion_1(word, clnt);
    if (!res) {
        clnt_perror(clnt, "insertion");
        exit(1);
    }
    return *res;
}

static int rpc_suppression(CLIENT *clnt, char *word)
{
    int *res = suppression_1(word, clnt);
    if (!res) {
        clnt_perror(clnt, "suppression");
        exit(1);
    }
    return *res;
}

static int rpc_cherche(CLIENT *clnt, char *word)
{
    int *res = cherche_1(word, clnt);
    if (!res) {
        clnt_perror(clnt, "cherche");
        exit(1);
    }
    return *res;
}

static int rpc_nbmots(CLIENT *clnt)
{
    int *res = nbmots_1(clnt);
    if (!res) {
        clnt_perror(clnt, "nbmots");
        exit(1);
    }
    return *res;
}

int main(int argc, char *argv[])
{
    char word[TAILLEMOT + 1];
    char cmd;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <serveur distant>\n", argv[0]);
        return 1;
    }

    CLIENT *clnt = clnt_create(argv[1], MMOTSPROG, MMOTSVERS, "tcp");
    if (!clnt) {
        clnt_pcreateerror(argv[1]);
        return 1;
    }

    for (;;) {
        printf("\n(I)nitialiser,(i)nsérer,(s)upprimer,(c)hercher,(n)ombre,(q)uitter\n");
        read_command(&cmd, word);
        switch (cmd) {
        case 'I':
            rpc_initialise(clnt);
            printf("Dictionnaire distant réinitialisé.\n");
            break;
        case 'i':
            rpc_insertion(clnt, word);
            printf("%s ajouté côté serveur.\n", word);
            break;
        case 's':
            if (rpc_suppression(clnt, word))
                printf("%s supprimé sur le serveur.\n", word);
            else
                printf("%s absent.\n", word);
            break;
        case 'c':
            if (rpc_cherche(clnt, word))
                printf("%s trouvé côté serveur.\n", word);
            else
                printf("%s absent.\n", word);
            break;
        case 'n':
            printf("Total distant: %d\n", rpc_nbmots(clnt));
            break;
        case 'q':
            clnt_destroy(clnt);
            return 0;
        default:
            printf("commande %c invalide (I,i,s,c,n,q).\n", cmd);
            break;
        }
    }
}
