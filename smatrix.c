#include "smatrix.h"
#include <stdio.h>

smatrix *createSmatrix(char *s) {
    return NULL;
}

int compatibleDimensions (smatrix *a, smatrix *b) {
    if (a==NULL || b==NULL)
        return -1;
    
    return ((a->m)==b->n);
}	

/* Pre : dimentions compatibles
 * Post : renvoie le produit des deux
 */

smatrix *product (smatrix *a, smatrix *b) {
    if (!compatibleDimensions(a,b)) {
        // rajouter un error
        return NULL;
    }
    
    // résultat
    smatrix *r = (smatrix *)malloc(sizeof(smatrix));
    if (r==NULL)
        return(EXIT_FAILURE);
    int nLines = a->n;
    int nColumns = b->m;
    r->n = nLines;
    r->m = nColumns;
    r->pointers = (queue **)malloc(nLines*sizeof(queue *));
    int i;
    for (i = 0; i< nLines; i++) {
        (a->pointers)[i] = createQueue();
    }
    
    
    int i, j;
    for (i=0; i<nLines; i++) {
        for (j= 0; j<nColumns; j++) {
            currentA = (a->pointer)[i]->first;
            currentB = (b->pointer)[j]->first;
            
            if (!(currentA || !currentB)
                enqueue((r->pointers)[i], j, 0);
        }
    }
}


