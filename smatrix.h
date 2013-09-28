#ifndef _smatrix_h
#define _smatrix_h

#include "queue.h"
#include <stdbool.h>

/* Matrice creuse */
typedef struct smatrix {
    /* Matrice de dimensions nxm */ // ATTENTION!! PAS DANS L'ORDRE ALPHABET.
    long n;
    long m;
    bool lines;
	queue **pointers;
} smatrix;

/* Crée une nouvelle matrice creuse à partir d'un char* de dimensions sous
 * la forme LxC, où L est le nombre de lignes et C celui de colonnes.
 * Le format doit être respecté et L et C doivent être > 0.
 * Le booléen indique si la matrice est stockée par lignes (true)
 * ou par colonnes (false).
 *
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL. */
smatrix *createSmatrix (char *, bool);

/* Renvoie 1 si les deux matrices peuvent être multipliées, 0 sinon.
 */
int compatibleDimensions (smatrix *, smatrix *);

/* Multiplie deux matrices creuses.
 *
 * Renvoie un pointeur vers la matrice résultante. En cas d'erreur,
 * retourne NULL.
 */
smatrix *product (smatrix *, smatrix *);

void freeSmatrix (smatrix *);

void displayPreSmatrix (smatrix *);

#endif

