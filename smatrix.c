#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smatrix.h"

/* Alloue de l'espace en mémoire pour créer une nouvelle matrice creuse.
 * lin et col indiquent le nombre de lignes et de colonnes respectivement,
 * b indique si la matrice est stockée par lignes (true)
 * ou par colonnes (false).
 *
 * Renvoie un pointeur vers la matrice. En cas d'erreur, retourne NULL.
 */
smatrix *allocate_smatrix(long lin, long col, bool b) {
    
    /* Vérifie que les dimensions sont cohérentes */
    if ((lin < 1) || (col < 1)) {
        fprintf(stderr, "Error calling allocate_smatrix :"
                " your matrix has dimensions %ldx%ld\n", lin, col);
        return NULL;
    }
    
    /* Crée la matrice */
    smatrix *matrix = (smatrix *)malloc(sizeof(smatrix));
    if (!matrix) {
        fprintf(stderr, "Error calling allocate_smatrix :"
                " malloc failed\n");
        return NULL;
    }
    
    /* Matrice stockée par lignes ou par colonnes */
    long store;
    if (b) {
        store = lin;
    } else {
        store = col;
    }
    
    /* Initialisation du tableau de listes chaînées */
    queue **q = (queue **)calloc((int) store, sizeof(queue *));
    if (!q) {
        fprintf(stderr, "Error calling allocate_smatrix :"
                " calloc to initialize the queue array failed\n");
        free_smatrix(matrix);
        return NULL;
    }
    
    /* Initialisation de toutes les listes chaînées */
    long i;
    for (i=0; i<store; i++) {
        q[i] = create_queue();
        if (!q[i]) {
            fprintf(stderr, "Error calling allocate_smatrix :"
                    " malloc to initialize queue %ld failed\n", i);
            free_smatrix(matrix);
            return NULL;
        }
    }
    
    /* Données de la matrice */
    matrix->n = lin;
    matrix->m = col;
    matrix->lines = b;
    matrix->pointers = q;
    
    /* Retourne une matrice initialisée */
    return matrix;
}


smatrix *create_smatrix(char *s, bool b) {
    
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
    strlcpy(dim1, s, length);
    
    
    /* Nombre de lignes */
    lin = strtol(dim1, &endptr, 10);
    
    /* Le format n'est pas respecté */
    if(*endptr !='x')
    {
        fprintf(stderr,"Error :  the file encoding is incorrect. Each matrix\n"
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
    strlcpy(dim2, endptr+1, length);
    
    /* Nombre de colonnes */
    col = strtol(dim2, &endptr, 10);
    if (*endptr != '\n') {
        fprintf(stderr,"Error :  the file encoding is incorrect. Each matrix\n"
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
    return allocate_smatrix(lin, col, b);
}


int fill_smatrix(smatrix *sm, long i, long j, long val) {
    if (!sm) {
        fprintf(stderr, "Error calling fillSMmatrix : the matrix is not initialised\n");
        return -1;
    }
    if ((i<0) || (j<0)) {
        fprintf(stderr, "Error calling fillSMmatrix : negative index values\n");
        return -1;
    }
    if (val == 0) {
        fprintf(stderr, "Error calling fillSMmatrix : null value in sparse\n");
        return 1;
    }
    
    if (sm->lines) {
        /* matrice stockée par lignes */
        return  enqueue((sm->pointers[i]), j, val);
    } else { /* matrice stockée par colonnes */
        return enqueue((sm->pointers[j]), i, val);
    }
}


/* Teste si 2 matrices peuvent être multipliées.
 *
 * Renvoit -1 si au moins un pointeur est nul
 *          1 si les deux matrices peuvent être multipliées
 *          0 sinon
 */
int compatible_dimensions (smatrix *a, smatrix *b) {
    if ((!a) || (!b)) {
        return -1;
    }
    
    return ((a->m)==(b->n));
}	


smatrix *smatrix_product (smatrix *a, smatrix *b, bool lines) {
    /* Test des dimensions */
    int cd = compatible_dimensions(a,b);
    // pointeur(s) NULL
    if (cd == -1) {
        fprintf(stderr, "Error calling smatrix_product :"
                " one of the pointers passed to smatrix_product is NULL\n");
        return NULL;
    }
    // dimensions incompatibles
    if (cd == 0) {
        fprintf(stderr, "Error calling smatrix_product : you're trying to"
                " multiply matrices with incompatible dimensions\n");
        return NULL;
    }
    
    /* Création de la matrice contenant le résultat de la multiplication */
    long nLines = a->n;
    long nColumns = b->m;
    long L, l;
    if (lines) {
        L = nLines;
        l = nColumns;
    }
    else {
        L = nColumns;
        l = nLines;
    }
    
    smatrix *r = allocate_smatrix(nLines, nColumns, lines);
    if (!r) {
        fprintf(stderr, "Error calling smatrix_product : malloc failed\n");
        return NULL;
    }
    
    // UTILISER LA FONTION CREATESMATRIX
    /*
    smatrix *r = (smatrix *)malloc(sizeof(smatrix));
    if (!r) {
        fprintf(stderr, "Error calling malloc to initialise the smatrix\n");
        return NULL;
    } 
    long nLines = a->n;
    long nColumns = b->m;
    r->n = nLines;
    r->m = nColumns;
    r->lines = lines;
    long L, l;
    if (lines) {
        L = nLines;
        l = nColumns;
    }
    else {
        L = nColumns;
        l = nLines;
    }
    
    r->pointers = (queue **)malloc(L*sizeof(queue *));
    if (r->pointers == NULL) {
        fprintf(stderr, "Error calling malloc to initialise the array of queues\n");
        free(r);
        return NULL;
    }
    long i;
    for (i = 0; i<L; i++) {
        (r->pointers)[i] = createQueue();
    }
     */
    // FIN DE LA FONCTION CREATEMATRIX

    /* Remplissage de la matrice */
    long i;
    long j, k, newV;
    for (i=0; i<nLines; i++) {
        for (j= 0; j<nColumns; j++) {
            newV = 0;
            node *currentA;
            node *currentB;
            
            /* parcourt la ligne i et la colonne j à la recherche
             * d'éléments à multiplier */
            for (currentA = (a->pointers)[i]->first;
                 currentA != NULL;
                 currentA = currentA->next) {
                
                k = currentA->j;
                
                for(currentB = (b->pointers)[j]->first;
                    currentB != NULL && currentB->j < k;
                    currentB = currentB->next) {
                    
                    /* ne fait rien */
                
                }
                if (currentB != NULL && currentB->j==k)
                    newV += (currentA->v)*(currentB->v);
            }
            /* ne rajoute un noeud à la queue que si sa valeur est non-nulle */
            if (newV != 0) {
                if (lines)
                    enqueue((r->pointers)[i], j, newV);
                else
                    enqueue((r->pointers)[j], i, newV);
            }
        }
    }
    
    /* Retourne la matrice résultante */
    return r;
}


void free_smatrix (smatrix *sm) {
    /* Matrice NULL: ne rien à faire */
    if (!sm) {
        return;
    }
    
    /* Libère pointers si il est non NULL */
    if (sm->pointers) {
        long i, l;
        if (sm->lines) {
            l = sm->n;
        } else {
            l = sm->m;
        }
        /* libère chaque queue de pointers */
        for (i=0; i<l; i++) {
            if (sm->pointers[i]) {
                free_queue(sm->pointers[i]);
                sm->pointers[i] = NULL;
            }
        }
        free(sm->pointers);
    }
    
    /* Libère la matrice */
    free(sm);
}
