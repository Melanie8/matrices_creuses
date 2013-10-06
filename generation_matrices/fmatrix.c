/*
 * Written by :
 * Lena Peschke <lena.peschke@student.uclouvain.be>
 * Mélanie Sedda <melanie.sedda@student.uclouvain.be>
 * October 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fmatrix.h"

/* Alloue de l'espace en mémoire pour créer une nouvelle matrice.
 * lin et col indiquent le nombre de lignes et de colonnes respectivement.
 *
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL.
 */
fmatrix *allocate_fmatrix(long lin, long col) {
    
    /* Vérifie que les dimensions sont cohérentes */
    if ((lin < 1) || (col < 1)) {
        fprintf(stderr, "Error calling allocate_fmatrix :"
                " your fmatrix has dimensions %ldx%ld\n", lin, col);
        return NULL;
    }
    
    /* Crée la matrice */
    fmatrix *mat = (fmatrix *)malloc(sizeof(fmatrix));
    if (!mat) {
        fprintf(stderr, "Error calling allocate_fmatrix :"
                " malloc failed\n");
        return NULL;
    }
    
    /* Crée le tableau */
    long **tab = (long **)malloc(lin*sizeof(long *));
    if (!tab) {
        fprintf(stderr, "Error calling allocate_fmatrix :"
                " malloc failed\n");
        return NULL;
    }
    
    /* Initialisation de toutes les listes chaînées */
    long i;
    for (i=0; i<lin; i++) {
        tab[i] = (long *)malloc(col*sizeof(long));
        if (!tab[i]) {
            fprintf(stderr, "Error calling allocate_fmatrix :"
                    " malloc to initialize line %ld failed\n", i);
            free_fmatrix(mat);
            return NULL;
        }
    }
    
    /* Données de la matrice */
    mat->n = lin;
    mat->m = col;
    mat->pointers = tab;
    
    /* Retourne une matrice initialisée */
    return mat;
    
}

fmatrix *create_fmatrix(char *s) {
    
    /* Extrait les dimensions de la matrice de s.
     Elles doivent être sous la forme 'lignes'x'colonnes'. */
    long lin, col;
    char *endptr;
    
    /* String dans lequel on copie la chaîne de caractères complète */
    size_t length = strlen(s)+1;
    char *dim1 = (char *) malloc(length*sizeof(char));
    if (!dim1) {
        fprintf(stderr, "Error calling malloc to handle the"
                " dimensions' string\n");
        return NULL;
    }
    strncpy(dim1, s, length);
    
    
    /* Nombre de lignes */
    lin = strtol(dim1, &endptr, 10);
    
    /* Le format n'est pas respecté */
    if(*endptr !='x')
    {
        fprintf(stderr,"Error :  the file encoding is incorrect. Each fmatrix\n"
                "\t should start with a dimensions' line as follows :\n"
                "\t LxC where L and C are the number of lines and columns. \n");
        free(dim1); dim1 = NULL;
        return NULL;
    }
    
    /* String dans lequel on copie le reste de la chaîne de caractères */
    length = strlen(endptr)+1;
    char *dim2 = (char *) malloc(length*sizeof(char));
    if (!dim2) {
        fprintf(stderr, "Error calling malloc to handle the"
                " dimensions' string\n");
        free(dim1); dim1 = NULL;
        return NULL;
    }
    strncpy(dim2, endptr+1, length);
    
    /* Nombre de colonnes */
    col = strtol(dim2, &endptr, 10);
    if (*endptr != '\n') {
        fprintf(stderr,"Error :  the file encoding is incorrect. Each fmatrix\n"
                "\t should start with a dimensions' line as follows :\n"
                "\t LxC where L and C are the number of lines and columns. \n"
                "\t Something was written beyond the second dimension.\n");
        free(dim1); dim1 = NULL;
        free(dim2); dim2 = NULL;
        return NULL;
    }
    
    free(dim1); dim1 = NULL;
    free(dim2); dim2 = NULL;
    
    /* Initialisation de la matrice */
    return allocate_fmatrix(lin, col);
    
}


/* Teste si 2 matrices peuvent être multipliées.
 *
 * Renvoit -1 si au moins un pointeur est nul
 *          1 si les deux matrices peuvent être multipliées
 *          0 sinon
 */
int compatible_dimensions (fmatrix *a, fmatrix *b) {
    if ((!a) || (!b)) {
        return -1;
    }
    
    return ((a->m)==(b->n));
}


fmatrix *fmatrix_product (fmatrix *a, fmatrix *b) {
    /* Test des dimensions */
    int cd = compatible_dimensions(a,b);
    // pointeur(s) NULL
    if (cd == -1) {
        fprintf(stderr, "Error calling fmatrix_product :"
                " one of the pointers passed to fmatrix_product is NULL\n");
        return NULL;
    }
    // dimensions incompatibles
    if (cd == 0) {
        fprintf(stderr, "Error calling fmatrix_product : you're trying to"
                " multiply matrices with incompatible dimensions\n");
        return NULL;
    }
    
    /* Création de la matrice contenant le résultat de la multiplication */
    long nLines = a->n;
    long nColumns = b->m;
    long nK = a->m;
    fmatrix *r = allocate_fmatrix(nLines, nColumns);
    
    /* Remplissage de la matrice */
    r->n = nLines;
    r->m = nColumns;
    
    long i, j, k, newV;
    for (i=0; i<nLines; i++) {
        for (j=0; j<nColumns; j++) {
            newV = 0;
            for(k=0; k<nK; k++) {
                newV += (a->pointers)[i][k]*(b->pointers)[k][j];
            }
            r->pointers[i][j] = newV;
        }
    }
    
    /* Retourne la matrice résultante */
    return r;
}


void free_fmatrix (fmatrix *sm) {
    /* Matrice NULL: ne rien à faire */
    if (!sm) {
        return;
    }
    
    /* Libère pointers si il est non NULL */
    if (sm->pointers) {
        long i;
        /* libère chaque queue de pointers */
        for (i=0; i<sm->n; i++) {
            if (sm->pointers[i]) {
                free(sm->pointers[i]);
                sm->pointers[i] = NULL;
            }
        }
        free(sm->pointers);
    }
    
    /* Libère la matrice */
    free(sm);
}
