/*
 * Written by :
 * Lena Peschke <lena.peschke@student.uclouvain.be>
 * Mélanie Sedda <melanie.sedda@student.uclouvain.be>
 * October 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fmatrix.h"

int convert_lines(fmatrix *sm, FILE *f) {
    
    long i, j; /* variables d'itération */
    long val; /* entrée de la matrice */
    long count = 0; /* nombre d'éléments total */
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    /* Boucle sur les lignes */
    for (i=0; i<sm->n; i++) {
        
        /* lit la prochaine ligne du fichier et la stocke dans line */
        read = getline(&line, &len, f);
        if (read <= 0) {
            perror("convert_line(getline failed)\n");
            if (line) {
                free(line); line = NULL;
            }
            return -1;
        }
        
        /* recopie la ligne dans un buffer */
        size_t length = strlen(line)+1;
        char *content = (char *)malloc(length*sizeof(char));
        if (!content) {
            fprintf(stderr, "Error calling convert_line : "
                    " malloc failed\n");
            if (line) {
                free(line); line = NULL;
            }
            return -1;
        }
        /* retient l'adresse de départ de l'espace alloué */
        char *remember = content;
        
        /* copie la ligne courante dans le buffer */
        strncpy(content, line, length);
        
        /* Boucle sur les colonnes */
        for (j=0; j<sm->m; j++) {
            
            /* convertit le char en long */
            val = strtol(content, &content, 10);
            count++;
            
            (sm->pointers)[i][j] = val;
        }
        free(remember); remember = NULL;
    }
    free(line); line = NULL;
    
    return count;
}


int print_fmatrix (fmatrix *sm, FILE* f) {
    if (!sm) {
        fprintf(stderr, "Error calling print_fmatrix : NULL pointer\n");
        return -1;
    }
    
    /* Ecrit les dimensions de la matrice */
    if (fprintf(f, "%ldx%ld\n", sm->n, sm->m) < 0) {
        perror("print_fmatrix(fprintf)");
        return -1;
    }
    
    long lin, col; /* variables d'itération */
    
    /* Boucle sur les lignes de la matrice */
    for (lin=0; lin<sm->n; lin++) {
        
        /* Boucle sur les colonnes de la matrice */
        for (col=0; col<sm->m; col++) {
            if (fprintf(f, "%ld ", (sm->pointers)[lin][col]) < 0)  {
                perror("print_fmatrix(fprintf)");
                return -1;
            }
        }
        
        /* Retour à la ligne pour marquer la ligne suivante */
        if (fprintf(f, "\n") < 0)  {
            perror("print_fmatrix(fprintf)");
            return -1;
        }
    }
    
    return 0;
}
