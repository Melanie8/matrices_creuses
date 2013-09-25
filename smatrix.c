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
    int i;
    for (i=0; i<(a->n); i++) {
        currentLine =
    }
}


