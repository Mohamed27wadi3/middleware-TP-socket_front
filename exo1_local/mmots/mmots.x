/* mmots.x */

/* Declarations RPCL pour rpcgen */

const TAILLEMOT = 50;           /* Taille maximale d'un mot du dictionnaire */
const TAILLEDICTIONNAIRE = 100; /* Nombre d'entrees du dictionnaire */

struct example {
        int     exfield1;
        char    exfield2;
};

/*--------------------------------------------------------------------
 * Programme distant de gestion du dictionnaire
 *--------------------------------------------------------------------
 */
program MMOTSPROG {
    version MMOTSVERS {
        int INITIALISE(void)  = 1;  /* premiere procedure du programme */
        int INSERTION(string) = 2;  /* seconde procedure */
        int SUPPRESSION(string) = 3;/* troisieme procedure */
        int CHERCHE(string) = 4;    /* quatrieme procedure */
        int NBMOTS(void) = 5;       /* retourne le nombre de mots */
    } = 1;
} = 0x30090949;
