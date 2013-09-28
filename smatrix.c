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
    queue **q = (queue **) calloc((int)store,sizeof(queue *));
    if (!q) {
        fprintf(stderr, "Error calling calloc to create a new queue for"
                " the new matrix\n");
        free(matrix);
        return NULL;
    }
    
    matrix->n = lin;
    matrix->m = col;
    matrix->lines = b;
    matrix->pointers = q;
   
    return matrix;
}




int compatibleDimensions (smatrix *a, smatrix *b) {
    if (a==NULL || b==NULL)
        return -1;
    
    return ((a->m)==b->n);
}	

/* Pre : dimensions compatibles
 * Post : renvoie le produit des deux
 */

smatrix *product (smatrix *a, smatrix *b) {
    if (!compatibleDimensions(a,b)) {
        // rajouter une error
        return NULL;
    }
    
    // résultat
    smatrix *r = (smatrix *)malloc(sizeof(smatrix));
    if (r==NULL)
        // rajouter une error
        return r;
    int nLines = a->n;
    int nColumns = b->m;
    r->n = nLines;
    r->m = nColumns;
    r->pointers = (queue **)malloc(nLines*sizeof(queue *)); //nb : si on parallélise, il faut faire nColumns*... si on veut que ca soit une postmatrice 
    
    int i;
    for (i = 0; i< nLines; i++) {
        (r->pointers)[i] = createQueue();
    }
    
    // on parcourt a en lignes et b en colonnes
    int j, k, newV;
    for (i=0; i<nLines; i++) {
        for (j= 0; j<nColumns; j++) {
            newV = 0;
            node *currentA;
            node *currentB;
            
            // on parcourt la ligne i et la colonne j à la recherche d'élément à multiplier
            for (currentA = (a->pointers)[i]->first; currentA != NULL; currentA = currentA->next) {
                k = currentA->j;
                for(currentB = (b->pointers)[j]->first; currentB != NULL && currentB->j < k; currentB = currentB->next) {}
                if (currentB != NULL && currentB->j==k)
                    newV += (currentA->v)*(currentB->v);
            }
            if (newV != 0)
                enqueue((r->pointers)[i], j, newV);
        }
    }
    return r;
}

void freeSmatrix (smatrix *sm) {
    
    if (!sm) {
        return;
    } else if (!sm->pointers) {
    } else {
        long i, max;
        if (sm->lines) {
            max = sm->n;
        } else {
            max = sm->m;
        }
        for (i=0; i<max; i++) {

            if (sm->pointers[i]) {
                freeQueue(sm->pointers[i]);
                sm->pointers[i] = NULL;
            }
        }
        free(sm->pointers);
    }
    free(sm); sm = NULL;
}

void displayPreSmatrix (smatrix *sm) {
    int n = sm->n;
    int m = sm->m;
    int i, j, k;
    for (i=0; i<n; i++) {
        node *current;
        j = -1;
        for (current = sm->pointers[i]->first; current!=NULL; current = current->next) {
            for (k=1; k<(current->j)-j; k++) {
                printf("0 ");
            }
            printf("%d ", current->v);
            j = current->j;
        }
        for (k=1; k < m-j; k++) {
            printf("0 ");
        }
        printf("\n");
    }
}


