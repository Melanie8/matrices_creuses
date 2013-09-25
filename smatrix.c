#include "smatrix.h"
#include <stdio.h>
#include "string.h"

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
    
    return NULL;
}


