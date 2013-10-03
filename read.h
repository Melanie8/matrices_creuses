#ifndef _read_h
#define _read_h

#include "smatrix.h"

/* Lit les lignes du fichier dont le descripteur de fichier est passé en
 * argument. Pour chaque ligne, convertit les valeurs chiffrées trouvées
 * en nombres pour les ajouter à la matrice dans la ligne correspondante
 * en tenant compte de la colonne de la valeur. Si une valeur nulle est lue
 * elle est ignorée.
 *
 * Retourne le nombre d'entrée de la matrice rencontrées dans le fichier,
 * même celles qui sont nulles.
 */
long convert_lines(smatrix *, FILE *);


/* Ecrit la matrice donnée en argument sur le stream donné. Il peut s'agir d'un
 * fichier ou de la sortie std.
 *
 * Retourne -1 en cas d'erreur, 0 sinon.
 */
int print_smatrix (smatrix *, FILE*);

#endif
