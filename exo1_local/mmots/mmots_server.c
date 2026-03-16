/*
 * Code intermediaire d'apres rpcgen
 */

#include "mmots.h"

int *initialise_1_svc(struct svc_req *rqstp)
{
    static int resultat;
    resultat = initialise();
    return &resultat;
}

int *insertion_1_svc(char *argp, struct svc_req *rqstp)
{
    static int resultat;
    resultat = insertion(argp);
    return &resultat;
}

int *suppression_1_svc(char *argp, struct svc_req *rqstp)
{
    static int resultat;
    resultat = suppression(argp);
    return &resultat;
}

int *cherche_1_svc(char *argp, struct svc_req *rqstp)
{
    static int resultat;
    resultat = cherche(argp);
    return &resultat;
}

int *nbmots_1_svc(struct svc_req *rqstp)
{
    static int resultat;
    resultat = nbmots();
    return &resultat;
}
