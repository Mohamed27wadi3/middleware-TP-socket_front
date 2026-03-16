#include <stdio.h>
#include <string.h>

#include "../common/mmots.h"

static char Dictionnaire[TAILLEDICTIONNAIRE][TAILLEMOT + 1];
static int CompteurMots = 0;

int initialise_logic(void)
{
    CompteurMots = 0;
    printf("[remote-server] dictionnaire vidé\n");
    return 1;
}

int insertion_logic(char *mot)
{
    strcpy(Dictionnaire[CompteurMots], mot);
    CompteurMots++;
    printf("[remote-server] ajout %s (total %d)\n", mot, CompteurMots);
    return CompteurMots;
}

int suppression_logic(char *mot)
{
    int i;
    for (i = 0; i < CompteurMots; i++) {
        if (strcmp(mot, Dictionnaire[i]) == 0) {
            CompteurMots--;
            strcpy(Dictionnaire[i], Dictionnaire[CompteurMots]);
            printf("[remote-server] suppression %s\n", mot);
            return 1;
        }
    }
    return 0;
}

int cherche_logic(char *mot)
{
    int i;
    for (i = 0; i < CompteurMots; i++)
        if (strcmp(mot, Dictionnaire[i]) == 0)
            return 1;
    return 0;
}

int nbmots_logic(void)
{
    return CompteurMots;
}
