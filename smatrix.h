// #ifndef _smatrix_h
// #define _smatrix_h

/* Matrice creuse */
typedef struct smatrix {
    /* Matrice de dimensions nxm */
    int n;
    int m;
	queue *pointers[];
} smatrix;

/* Crée une nouvelle matrice creuse à partir d'un fichier d'entrée.
 *
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL. */
smatrix *createSmatrix (char *);

/* Renvoie 1 si les deux matrices peuvent être multipliées, 0 sinon.
 */
int compatibleDimensions (smatrix *, smatrix *);

/* Multiplie deux matrices creuses.
 *
 * Renvoie un pointeur vers la matrice résultante. En cas d'erreur, retourne NULL. 
 */
smatrix *product (smatrix *, smatrix *);

//#endif

