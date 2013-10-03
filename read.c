#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "smatrix.h"

int convert_lines(smatrix *sm, FILE *f) {
    
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
        strlcpy(content, line, length);
        
        /* Boucle sur les colonnes */
        for (j=0; j<sm->m; j++) {
            
            /* convertit le char en long */
            val = strtol(content, &content, 10);
            count++;
            
            if (val == 0) {
                /* si la valeur est 0, on ne la retien pas */
                
            } else {
                /* sinon, on l'ajoute à la matrice */
                if (fill_smatrix(sm, i, j, val) != 0) {
                    fprintf(stderr, "Error calling convert_line : "
                            " fill_smatrix failed\n");
                    if (line) {
                        free(line); line = NULL;
                    }
                    if (content) {
                        free(remember); remember = NULL;
                    }
                    return -1;
                }
            }
        }
        free(remember); remember = NULL;
    }
    free(line); line = NULL;
    
    return count;
}


int print_smatrix (smatrix *sm, FILE* f) {
    if (!sm) {
        fprintf(stderr, "Error calling print_smatrix : NULL pointer\n");
        return -1;
    }
    
    if (!(sm->lines)) {
        fprintf(stderr, "Error calling print_smatrix : right matrix"
                " cannot be printed\n");
        return -1;
    }
    
    /* Ecrit les dimensions de la matrice */
    if (fprintf(f, "%ldx%ld\n", sm->n, sm->m) < 0) {
        perror("print_smatrix(fprintf)");
        return -1;
    }
    
    long lin, col; /* variables d'itération */
    
    /* Boucle sur les lignes de la matrice */
    for (lin=0; lin<sm->n; lin++) {
        node *current;
        long diff = -1;
        
        /* Boucle sur les la liste chaînée de la ligne */
        for (current = sm->pointers[lin]->first; current!=NULL;
             current = current->next) {
            
            /* Zéros éventuels avant le prochain élément non-nul */
            for (col=1; col<((current->j)-diff); col++) {
                if (fprintf(f, "0 ") < 0) {
                    perror("print_smatrix(fprintf)");
                    return -1;
                }
            }
            
            /* Ecriture de l'élément trouvé */
            if (fprintf(f, "%ld ", current->v) < 0)  {
                perror("print_smatrix(fprintf)");
                return -1;
            }
            diff = current->j;
        }
        
        /* Zéros éventuels après le précédent élément non-nul */
        for (col=1; col<(sm->m - diff); col++) {
            if (fprintf(f, "0 ") < 0 )  {
                perror("print_smatrix(fprintf)");
                return -1;
            }
        }
        
        /* Retour à la ligne pour marquer la ligne suivante */
        if (fprintf(f, "\n") < 0)  {
            perror("print_smatrix(fprintf)");
            return -1;
        }
    }
    
    return 0;
}
