/*
 * Written by :
 * Lena Peschke <lena.peschke@student.uclouvain.be>
 * Mélanie Sedda <melanie.sedda@student.uclouvain.be>
 * October 2013
 */

#ifndef _read_h
#define _read_h

#include "fmatrix.h"

/* Lit les lignes du fichier dont le descripteur de fichier est passé en
 * argument. Pour chaque ligne, convertit les valeurs chiffrées trouvées
 * en nombres pour les ajouter à la matrice dans la ligne correspondante
 * en tenant compte de la colonne de la valeur. Si une valeur nulle est lue
 * elle est ignorée.
 *
 * Retourne le nombre d'entrée de la matrice rencontrées dans le fichier,
 * même celles qui sont nulles.
 */
long convert_lines(fmatrix *, FILE *);


/* Ecrit la matrice donnée en argument sur le stream donné. Il peut s'agir
 * d'un fichier ou de la sortie std.
 *
 * Retourne -1 en cas d'erreur, 0 sinon.
 */
int print_fmatrix (fmatrix *, FILE*);

#endif
