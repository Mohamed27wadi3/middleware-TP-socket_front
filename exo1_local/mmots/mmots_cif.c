/* mmots_cif.c - initialise, insertion, suppression, cherche, nbmots */

#include <rpc/rpc.h>
#include <stdio.h>

#include "mmots.h"
/* Talon (stub) cote client ecrit a la main */

extern CLIENT *handle; /* handle de la procedure serveur */

/*------------------------------------------------------------------------
 * initialise - procedure interface client qui appelle initialise_1
 *------------------------------------------------------------------------
 */
int initialise()
{
   return *initialise_1(handle);
}

/*------------------------------------------------------------------------
 * insertion - procedure interface client qui appelle insertion_1
 *------------------------------------------------------------------------
 */
int insertion(char* Mot)
{
   return *insertion_1(Mot, handle);
}

/*------------------------------------------------------------------------
 * suppression - procedure interface client qui appelle suppression_1
 *------------------------------------------------------------------------
 */
int suppression(char* Mot)
{
   return *suppression_1(Mot, handle);
}

/*------------------------------------------------------------------------
 * cherche - procedure interface client qui appelle cherche_1
 *------------------------------------------------------------------------
 */
int cherche(char* Mot)
{
   return *cherche_1(Mot, handle);
}

/*------------------------------------------------------------------------
 * nbmots - procedure interface client qui appelle nbmots_1
 *------------------------------------------------------------------------
 */
int nbmots()
{
   return *nbmots_1(handle);
}
