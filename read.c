#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smatrix.h"

int convertLines(smatrix *sm, FILE *f) {
    
    long i, j; // variables d"itération
    long val;
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    for (i=0; i<sm->n; i++) {
        
        /* lit la prochaine ligne du fichier et la stocke dans line */
        read = getline(&line, &len, f);
        fwrite(line, read, 1, stdout);
        
        if (read <= 0) {
            fprintf(stderr, "Error calling getline to get the matrix values\n");
            if (line) {
                free(line);
                line = NULL;
            }
            return -1;
        }
        
        /* recopie la ligne dans un buffer */
        size_t length = strlen(line)+1;
        char *content = (char *) malloc(length*sizeof(char));
        if (!content) {
            fprintf(stderr, "Error calling malloc to read the file\n");
            if (line) {
                free(line);
                line = NULL;
            }
            return -1;
        }
        strlcpy(content, line, length);
        
        char *endptr;
        for (j=0; j<sm->m; j++) {
            
            /* convertit le char en long */
            val = strtol(content, &endptr, 10);
            printf("valeur récupérée : %ld\n", val);
            
            if (val == 0) {
                /* si la valeur est 0, on ne la retien pas */

            } else {
                /* sinon, on l'ajoute à la matrice */
                printf("appel de convertline %ldx%ld avec val %ld\n",i,j, val);
                
                if (fillSmatrix(sm, i, j, val) != 0) {
                    fprintf(stderr, "Error calling fillSMmatrix\n");
                    if (line) {
                        free(line);
                        line = NULL;
                    }
                    if (content) {
                        free(content);
                        content = NULL;
                    }
                    return -1;
                }
            }
            /* copie le reste de la ligne dans le buffer */
            strlcpy(content, endptr, strlen(content)+1);
        }
        free(content);
        content = NULL;
    }
    free(line);
    line = NULL;

    return 0;
}

/*
int i printSmatrix (smatrix *sm) {
    if (!sm) {
        fprintf("Error calling printSmatrix: NULL pointer\n");
        return -1;
    }
    
    long lin, col;
    if (sm->lines) {
 // il faut parcourir la matrice dans l'ordre des pointers
        for (lin=0; i<sm->n; i++) {
            
            // parcourt las noeuds de la liste chaînée (parcourt une ligne)
            node *current;
            diff = -1;
            for (current = sm->pointers[i]->first; current!=NULL; current = current->next) {
                for (col=1; col<((current->j)-diff); col++) {
                    printf("0 "); //
                }
                printf("%ld ", current->v); //
                diff = current->j;
            }
            for (col=1; col<(sm->m - diff); col++) {
                printf("0 "); //
            }
            printf("\n"); //
        }
    } else {
        // il faut parcourir la matrice dans l'autre ordre
        for (lin=0; i<sm->n; i++) {
            for (col=0; col<sm->m; <col++) {
                
            }
        }
    }
    
                
                
            
        }
    }
    

}
*/

