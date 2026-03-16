/* mmots_srp.c - initialise, insertion, suppression, cherche, nbmots */

#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include "mmots.h"

/* Procedures distantes cote serveur avec leurs donnees */

char Dictionnaire[TAILLEDICTIONNAIRE][TAILLEMOT+1]; /* dictionnaire de mots */
int  CompteurMots = 0; /* nombre de mots dans le dictionnaire */

/*------------------------------------------------------------------------
 * initialise - initialise le dictionnaire avec 0 mots
 *------------------------------------------------------------------------
 */
int initialise()
{
    CompteurMots = 0;
    printf("Initialisation du dictionnaire ...\n");
    return 1;
}

/*------------------------------------------------------------------------
 * insertion - insere un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int insertion(char *Mot)
{
    strcpy(Dictionnaire[CompteurMots], Mot);
    CompteurMots++;
    return CompteurMots;
}

/*------------------------------------------------------------------------
 * suppression - supprime un mot du dictionnaire
 *------------------------------------------------------------------------
 */
int suppression(char *Mot)
{
    int i;

    for (i = 0; i < CompteurMots; i++)
        if (strcmp(Mot, Dictionnaire[i]) == 0) {
            CompteurMots--;
            strcpy(Dictionnaire[i], Dictionnaire[CompteurMots]);
            return 1;
        }
    return 0;
}

/*------------------------------------------------------------------------
 * cherche - cherche un mot dans le dictionnaire
 *------------------------------------------------------------------------
 */
int cherche(char *Mot)
{
    int i;

    for (i = 0; i < CompteurMots; i++)
        if (strcmp(Mot, Dictionnaire[i]) == 0)
            return 1;
    return 0;
}

/*------------------------------------------------------------------------
 * nbmots - retourne le nombre courant de mots
 *------------------------------------------------------------------------
 */
int nbmots()
{
    return CompteurMots;
}
