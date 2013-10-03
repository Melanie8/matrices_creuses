#ifndef _smatrix_h
#define _smatrix_h

#include <stdbool.h>
#include "queue.h"

/* Exemple illustrant notre encodage d'une matrice creuse :
 *
 * Soit la matrice 0 1 0
 *                 1 1 0
 *
 * Dans notre structure smatrix, nous retenons tout d'abord les dimensions
 * de notre matrice (dans ce cas-ci n = 2 et m = 3).
 *
 * Ensuite, nous enregistrons son contenu sous la forme d'un tableau de queues
 * (pointers). Le ième élément de ce tableau contient une queue dont les noeuds
 * représentent les éléments non nul de la ième ligne. Chaque noeud contient
 * une valeur v, un indice j et un pointeur vers le noeud suivant. Dans ce 
 * cas-ci, pointers[0] contient donc un noeud (v=1, j=1) et pointers [1]
 * contient deux noeuds (v=1, j=0) et (v=1, j=1).
 *
 * Pour plus de facilité lors de la multiplication de deux matrices A et B.
 * Nous
 * avons décidé d'encoder B par colonne et non par ligne. C'est pourquoi nous
 * avons ajouté un booléen lines qui vaut 1 si la matrice est encodée par
 * lignes et 0 si elle est encodée par colonnes.
 */
typedef struct smatrix_t {
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
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL.
 */
smatrix *create_smatrix (char *, bool);


/* Remplit une case de la matrice creuse. Le pointeur fait référence à une
 * smatrix initialisée par la fonction createSmatrix. Les trois valeurs long
 * représentent la position en ligne, en colonne et la valeur du champ.
 * Si la matrice est stockée en lignes, l'élément sera ajouté dans la ième
 * ligne. Si la matrice est stockée en colonne, l'élément sera placé dans 
 * la jème colonne. Dans les deux cas, il s'agit du tableau de listes chaînées
 * de la matrice creuse.
 *
 * Renvoit -1 si le pointeur est nul ou
 *            si une des valeurs d'index est négative
 *          1 si la valeur du champ est nulle
 */
int fill_smatrix(smatrix *, long, long, long);


/* Multiplie deux matrices creuses, dont la 1ère a été encodée par lignes et la 
 * 2ème par colonnes. Le booléen passé en argument spécifie si l'on veut que le
 * résulat soit encodé par lignes ou par colonnes.
 *
 * Renvoie un pointeur vers la matrice résultante. En cas d'erreur
 * (pointeur NULL, dimensions incompatibles, échec d'un malloc), retourne NULL.
 */
smatrix *smatrix_product (smatrix *, smatrix *, bool);


/* Supprime une matrice creuse de la mémoire.
 */
void free_smatrix (smatrix *);

#endif

