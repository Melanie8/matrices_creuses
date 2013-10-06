/*
 * Written by :
 * Lena Peschke <lena.peschke@student.uclouvain.be>
 * Mélanie Sedda <melanie.sedda@student.uclouvain.be>
 * October 2013
 */

#ifndef _fmatrix_h
#define _fmatrix_h

#include <stdbool.h>

/* Matrice pleine
 */
typedef struct fmatrix_t {
    long n;
    long m;
	long **pointers;
} fmatrix;

/* Alloue de l'espace en mémoire pour créer une nouvelle matrice.
 * lin et col indiquent le nombre de lignes et de colonnes respectivement.
 *
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL.
 */
fmatrix *allocate_fmatrix(long, long);

/* Crée une nouvelle matrice pleine à partir d'un char* de dimensions sous
 * la forme LxC, où L est le nombre de lignes et C celui de colonnes.
 * Le format doit être respecté et L et C doivent être > 0.
 *
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL.
 */
fmatrix *create_fmatrix (char *);


/* Multiplie deux matrices creuses, dont la 1ère a été encodée par lignes et la
 * 2ème par colonnes. 
 *
 * Renvoie un pointeur vers la matrice résultante. En cas d'erreur
 * (pointeur NULL, dimensions incompatibles, échec d'un malloc), retourne NULL.
 */
fmatrix *fmatrix_product (fmatrix *, fmatrix *);


/* Supprime une matrice creuse de la mémoire.
 */
void free_fmatrix (fmatrix *);

#endif
