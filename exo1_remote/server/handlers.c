#include "../common/mmots.h"

/* Fonctions metier implementées dans logic.c */
int initialise_logic(void);
int insertion_logic(char *mot);
int suppression_logic(char *mot);
int cherche_logic(char *mot);
int nbmots_logic(void);

int *initialise_1_svc(struct svc_req *rqstp)
{
    static int res;
    res = initialise_logic();
    return &res;
}

int *insertion_1_svc(char *mot, struct svc_req *rqstp)
{
    static int res;
    res = insertion_logic(mot);
    return &res;
}

int *suppression_1_svc(char *mot, struct svc_req *rqstp)
{
    static int res;
    res = suppression_logic(mot);
    return &res;
}

int *cherche_1_svc(char *mot, struct svc_req *rqstp)
{
    static int res;
    res = cherche_logic(mot);
    return &res;
}

int *nbmots_1_svc(struct svc_req *rqstp)
{
    static int res;
    res = nbmots_logic();
    return &res;
}
