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
#include "queue.h"


int main(int argc, char *argv[]) {
    
    // matrice creuse a
    smatrix *a = (smatrix *)malloc(sizeof(smatrix));
    if (a==NULL)
        return(EXIT_FAILURE);
    
    a->n = 2;
    a->m = 3;
    a->pointers = (queue **)malloc((a->n)*sizeof(queue *));
    int i;
    for (i = 0; i< (a->n); i++) {
        (a->pointers)[i] = createQueue();
    }
    
    enqueue((a->pointers)[0], 1, 1);
    enqueue((a->pointers)[1], 0, 1);
    enqueue((a->pointers)[1], 1, 1);
    
    // test sur a
    printf("%d\n", (a->pointers)[1]->first->j);
    
    // matrice creuse b
    smatrix *b = (smatrix *)malloc(sizeof(smatrix));
    if (b==NULL)
        return(EXIT_FAILURE);
    
    b->n = 3;
    b->m = 3;
    b->pointers = (queue **)malloc((b->m)*sizeof(queue *));
    for (i = 0; i< (b->m); i++) {
        (b->pointers)[i] = createQueue();
    }
    
    enqueue((b->pointers)[1], 0, 1);
    enqueue((b->pointers)[1], 1, 1);
    enqueue((b->pointers)[2], 2, 1);
    
    // test sur a
    printf("%d\n", (b->pointers)[2]->first->j);
    
    return (EXIT_SUCCESS);
}

