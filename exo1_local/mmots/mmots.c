/*
 * Client RPC dictionnaire - utilise mmots.x via rpcgen
 */

#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "mmots.h"

#define TAILLEMOT 50           /* Taille maximale d'un mot du dictionnaire */
#define RSERVEUR "localhost"  /* nom du remote host serveur par defaut */

CLIENT *handle;                /* handle de la procedure serveur */

static int CommandeSuivante(char *Commande, char *Mot);

/*------------------------------------------------------------------------
 * main - insere, supprime, cherche ou compte un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
    char Mot[TAILLEMOT + 1];   /* allocation de la memoire pour un mot */
    char Commande;             /* commande saisie */
    int LongueurMot;           /* longueur du mot saisi */
    const char *hote = (argc > 1) ? argv[1] : RSERVEUR;

    /* connexion avec le serveur pour les appels RPC  */
    handle = clnt_create(hote, MMOTSPROG, MMOTSVERS, "tcp");
    if (handle == NULL) {
        printf("Connexion impossible avec %s (programme distant).\n", hote);
        exit(1);
    }

    while (1) {
        printf("\n(I)nitialiser,(i)nsérer,(s)upprimer,(c)hercher,(n)ombre,(q)uitter\n");
        LongueurMot = CommandeSuivante(&Commande, Mot);

        switch (Commande) {
        case 'I': /* initialise */
            initialise();
            printf("Dictionnaire initialisé à vide.\n");
            break;

        case 'i': /* insertion */
            insertion(Mot);
            printf("%s inséré.\n", Mot);
            break;
        case 's': /* suppression */
            if (suppression(Mot))
                printf("%s supprimé.\n", Mot);
            else
                printf("%s non trouvé.\n", Mot);
            break;
        case 'c': /* cherche */
            if (cherche(Mot))
                printf("%s trouvé.\n", Mot);
            else
                printf("%s non trouvé.\n", Mot);
            break;
        case 'n': /* nombre de mots */
            printf("Nombre de mots dans le dictionnaire: %d\n", nbmots());
            break;
        case 'q': /* quit */
            printf("fin de session.\n");
            clnt_destroy(handle);
            exit(0);
        default: /* entrée erronée */
            printf("commande %c invalide (I,i,s,c,n,q).\n", Commande);
            break;
        }
    }
}

/*------------------------------------------------------------------------
 * CommandeSuivante  - lit une commande et (éventuellement) un mot
 *------------------------------------------------------------------------
 */
static int CommandeSuivante(char *Commande, char *Mot)
{
    printf("Commande: ");
    scanf("%s", Commande);
    if (*Commande != 'q' && *Commande != 'I' && *Commande != 'n') {
        printf("Mot: ");
        scanf("%50s", Mot);
        return (int)strlen(Mot);
    }
    return -1;
}
