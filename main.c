#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "smatrix.h"


int main(int argc, char *argv[]) {
    
    // matrice creuse a
    smatrix *a = (smatrix *)malloc(sizeof(smatrix));
    if (a==NULL)
        return(EXIT_FAILURE);
    
    a->n = 2;
    a->m = 3;
    int cola[3] = {1, 2, 0};
    a->columns = cola;
    int pointa[2] = {0, 2};
    a->pointers = pointa;
    
    // matrice creuse b
    smatrix *b = (smatrix *)malloc(sizeof(smatrix));
    if (b==NULL) {
        printf("fail malloc");
        return(EXIT_FAILURE);
    }
    b->n = 3;
    b->m = 3;
    int colb[2] = {1, 2};
    b->columns = colb;
    int pointb[3] = {-1, 0, 1};
    b->pointers = pointb;
    
    // test compatibleDimensions
    printf("%d", compatibleDimensions(a, b));
    
    return (EXIT_SUCCESS);
}

