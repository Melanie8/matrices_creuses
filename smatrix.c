#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smatrix.h"

// à compléter et améliorer, pas de 2ble code
smatrix *createSmatrix(char *s, bool b) {
    
    /* On extrait les dimensions de la matrice de s.
       Elles doivent être sous la forme 'lignes'x'colonnes'. */
    
    long lin, col;
    char *endptr;
    
    /* String dans lequel on copie la chaîne de caractères complète. */
    char *dim1 = (char *) malloc((strlen(s)+1)*sizeof(char));
    if (!dim1) {
        fprintf(stderr, "Error calling malloc to handle the"
                " dimensions' string\n");
        return NULL;
    }
    strcpy(dim1, s);
    
    /* Nombre de lignes. */
    lin = strtol(dim1, &endptr, 10);
    free(dim1);
    
    /* Le format n'est pas respecté. */
    if(*endptr !='x')
    {
        fprintf(stderr,"Error :  the file encoding is incorrect. Each matrix\n"
                "\t should start with a dimensions' line as follows :\n"
                "\t LxC where L and C are the number of lines and columns. \n");
        return NULL;
    }

    /* String dans lequel on copie le reste de la chaîne de caractères. */
    char *dim2 = (char *) malloc((strlen(endptr))*sizeof(char));
    if (!dim2) {
        fprintf(stderr, "Error calling malloc to handle the"
                " dimensions' string\n");
        return NULL;
    }
    strcpy(dim2, endptr+1);
    
    /* Nombre de colonnes */
    col = strtol(dim2, &endptr, 10);
    free(dim2);
    if (*endptr != '\0') {
        fprintf(stderr,"Error :  the file encoding is incorrect. Each matrix\n"
                "\t should start with a dimensions' line as follows :\n"
                "\t LxC where L and C are the number of lines and columns. \n"
                "\t Something was written beyond the second dimension.\n");
        return NULL;
    }
    
    /* On vérifie que les dimensions sont cohérentes. */
    if ((lin < 1) || (col < 1)) {
        fprintf(stderr, "Error :  your matrix has dimensions %ld x %ld\n",
                lin, col);
        return NULL;
    }
    
    
    /* On crée la matrice. */
    
    smatrix *matrix = (smatrix *) malloc(sizeof(smatrix));
    if (!matrix) {
        fprintf(stderr, "Error calling malloc to create a new matrix\n");
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
    queue *q = createQueue();
    if (!q) {
        fprintf(stderr, "Error calling createQueue to create a new queue for"
                " the new matrix\n");
        free(matrix);
        return NULL;
    }
    
    matrix->n = lin;
    matrix->m = col;
    matrix->lines = b;
    matrix->pointers = &q;
   
    return matrix;
}

int fillSmatrix(smatrix *sm, long i, long j, long val) {
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
    
    int err;
    if (sm->lines) { // matrice en lignes
        err = enqueue((sm->pointers[i]), j, val);
        return err;
    } else { // matrice en colonnes
        return (enqueue((sm->pointers[j]), i, val));
    }
}



int compatibleDimensions (smatrix *a, smatrix *b) {
    if (a==NULL || b==NULL)
        return -1;
    return ((a->m)==(b->n));
}	



smatrix *product (smatrix *a, smatrix *b, bool lines) {
    /* Test des dimensions */
    int cd = compatibleDimensions(a,b);
    // pointeur(s) NULL
    if (cd == -1) {
        fprintf(stderr, "Error : one of the pointers passed to the product fonction is NULL");
        return NULL;
    }
    // dimensions incompatibles
    if (cd == 0) {
        fprintf(stderr, "Error : you're trying to multiply matrices with incompatible dimensions");
        return NULL;
    }
    
    
    // UTILISER LA FONTION CREATESMATRIX
    /* Création de la matrice qui va contenir le résultat de la multiplication */
    smatrix *r = (smatrix *)malloc(sizeof(smatrix));
    if (r==NULL) {
        fprintf(stderr, "Error calling malloc to initialise the smatrix");
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
        fprintf(stderr, "Error calling malloc to initialise the array of queues");
        free(r);
        return NULL;
    }
    long i;
    for (i = 0; i<L; i++) {
        (r->pointers)[i] = createQueue();
    }

    /* Remplissage de la matrice */
    long j, k, newV;
    for (i=0; i<nLines; i++) {
        for (j= 0; j<nColumns; j++) {
            newV = 0;
            node *currentA;
            node *currentB;
            
            // on parcourt la ligne i et la colonne j à la recherche d'éléments à multiplier
            for (currentA = (a->pointers)[i]->first; currentA != NULL; currentA = currentA->next) {
                k = currentA->j;
                for(currentB = (b->pointers)[j]->first; currentB != NULL && currentB->j < k; currentB = currentB->next) {}
                if (currentB != NULL && currentB->j==k)
                    newV += (currentA->v)*(currentB->v);
            }
            // on ne rajoute un noeud à la queue que si sa valeur n'est pas nulle
            if (newV != 0) {
                if (lines)
                    enqueue((r->pointers)[i], j, newV);
                else
                    enqueue((r->pointers)[j], i, newV);
            }
            
        }
    }
    
    /* Retour du résultat */
    return r;
}


void freeSmatrix (smatrix *sm) {
    /* Matrice NULL -> rien à faire */
    if (!sm) {
        return;
    }
    
    /* Libérer pointers si il est non NULL */
    if (sm->pointers) {
        long i, l;
        if (sm->lines) {
            l = sm->n;
        } else {
            l = sm->m;
        }
        for (i=0; i<l; i++) {
            if (sm->pointers[i]) {
                freeQueue(sm->pointers[i]);
                sm->pointers[i] = NULL;
            }
        }
        free(sm->pointers);
    }
    /* Libérer la matrice */
    free(sm);
}



void displaySmatrix (smatrix *sm) {
    if (!sm) {
        printf("Le pointeur passé en argument est NULL");
        return;
    }
    bool lines = sm->lines;
    long n, m;
    if (lines) {
        n = sm->n;
        m = sm->m;
    }
    else {
        n = sm->m;
        m = sm->n;
    }
    long i, j, k;
    for (i=0; i<n; i++) {
        node *current;
        j = -1;
        for (current = sm->pointers[i]->first; current!=NULL; current = current->next) {
            for (k=1; k<(current->j)-j; k++) {
                printf("0 ");
            }
            printf("%ld ", current->v);
            j = current->j;
        }
        for (k=1; k < m-j; k++) {
            printf("0 ");
        }
        printf("\n");
    }
}

