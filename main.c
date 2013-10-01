#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include "smatrix.h"
#include "queue.h"

int main(int argc, char *argv[]) {
    
    char *ifile; /* nom du fichier d'entrée */
    FILE *ides = NULL; /* descripteur de fichier associé au fichier d'entrée */
    char *ofile; /* nom du fichier de sortie */
    //FILE *odes = NULL; /* descripteur de fichier associé au fichier de sortie */
    
    int exit_status = EXIT_SUCCESS;
    
    
    /* Récupération des arguments */
    // vérifier le nombre d'arguments?
    int optch;
    while ((optch = getopt(argc, argv, "o:")) != -1)
        switch (optch) {
            case 'o':
                printf ("Paramètre o rencontré avec argument %s\n", optarg);
                ofile = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o output_file] input_file\n", argv[0]);
                exit_status = EXIT_FAILURE;
                goto end;
        }
    
    ifile = argv[optind];
    printf("infile: %s\n", ifile);
    
    /* Lecture du fichier et manipulation des matrices */
    
    /* ouverture du fichier infile */
    if ((ides = fopen(ifile, "r")) < 0) {
        perror("fopen(input_file)");
        exit_status = EXIT_FAILURE;
        goto end;
    }

    /* matrices utilisées pour les opérations */
    smatrix *left;
    smatrix *right;
    smatrix *result = NULL; // enlever
    smatrix *current;
    bool left_already = false;
    bool new_available = false;
    
    /* initialisation */
    long line, col, val;
	ssize_t read;
	size_t len = 0;
	char *snippet = NULL;
    
    /* boucle sur les lignes pour les nouvelles matrices */
	while ((read = getline(&snippet, &len, ides)) > 0) {
        printf("passé le 1er read : %s\n", snippet);
        
        /* multiplication sur la variable */
        // mult
        // swap entre resultat et left, right vide
        
        /* création d'une nouvelle matrice */
        if (!left_already) {
            printf("1e matrice pas encore creee\n");
            // matrice par lignes
            left = createSmatrix(snippet, true);
            printf("1e matrice creee\n");
            if (!left) {
                exit_status = EXIT_FAILURE;
                free(snippet); snippet = NULL;
                goto close_ifile;
            }
            current = left;
            left_already = true;
        } else {
            printf("hehe\n");
            printf("%s\n", snippet);
            // matrice par colonnes
            right = createSmatrix(snippet, false);
            printf("2e matrice creee\n");
            if (!right) {
                exit_status = EXIT_FAILURE;
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                goto free_matrixes;
            }
            current = right;
            new_available = true;
        }
        
        /* boucle sur les lignes pour remplir la matrice créée */
        for (line = 0; line < current->n ; line++) {
            printf("line %ld du fichier en lecture \n", line);
            /* on lit ligne par ligne */
            read = getline(&snippet, &len, ides);
            if (read <= 0) {
                fprintf(stderr, "Error calling getline \n");
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                exit_status = EXIT_FAILURE;
                goto free_matrixes;
            }
            printf("passé le 2e read : %s\n", snippet);
            // buffer qui contient le reste de la ligne à convertir en nombres
            char *endptr;
            
            // buffer qui contient la ligne en entier
            char *content = (char *) malloc(strlen((snippet+1))*sizeof(char));
            if (!content) {
                fprintf(stderr, "Error calling malloc to read the file\n");
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                exit_status = EXIT_FAILURE;
                goto free_matrixes;
            }
            strcpy(content, snippet);
            printf("passé le copy : %s\n", snippet);
            
            /* boucle sur la ligne pour récupérer les valeurs */
            for (col = 0; col < current->m; col++) {
                printf("col %ld du fichier en lecture \n", col);
                val = strtol(content, &endptr, 10);
                
                if (val == 0) {
                    // A REVOIR
                    // if the value is 0, do not enqueue
                } else {
                    printf("appel de fill avec val %ld\n", val);
                    
                    if (fillSmatrix(current, line, col, val) != 0) {
                        fprintf(stderr, "Error calling fillSMmatrix\n");
                        if (snippet) {
                            free(snippet); snippet = NULL;
                        }
                        if (content) {
                            free(content); content = NULL;
                        }
                        exit_status = EXIT_FAILURE;
                        goto free_matrixes;
                    }
                }
                
                // copy the rest of the line into the buffer
                strcpy(content, endptr);
            }
            free(content); content = NULL;
        }
    }
    free(snippet); snippet = NULL;
    
    
    displaySmatrix(left);
    displaySmatrix(right);
    displaySmatrix(current);
    
    
    /* fichier de sortie optionnel */
    
    
    
    /* SECTION TEST */
    /*
     char *s = "-1x4";
     char *t = "2x3";
     
     smatrix *matrixdim1 = createSmatrix(s, true);
     smatrix *matrixdim2 = createSmatrix(t, true);
     
     printf("le free commence\n");
     freeSmatrix(matrixdim1);
     freeSmatrix(matrixdim2);
     
     
     if (1)
     printf("1");
     if (0)
     printf("0");
     if (2)
     printf("2");
     
     // matrice creuse a
     smatrix *a = (smatrix *)malloc(sizeof(smatrix));
     if (a==NULL)
     return(EXIT_FAILURE);
     
     a->n = 2;
     a->m = 3;
     a->lines=1;
     a->pointers = (queue **)malloc((a->n)*sizeof(queue *));
     int i;
     for (i = 0; i< (a->n); i++) {
     (a->pointers)[i] = createQueue();
     }
     
     enqueue((a->pointers)[0], 1, 1);
     enqueue((a->pointers)[1], 0, 1);
     enqueue((a->pointers)[1], 1, 1);
     
     // test sur a
     printf("%ld\n", (a->pointers)[1]->first->next->j);
     
     // matrice creuse b
     smatrix *b = (smatrix *)malloc(sizeof(smatrix));
     if (b==NULL)
     return(EXIT_FAILURE);
     
     b->n = 3;
     b->m = 3;
     b->lines = 0;
     b->pointers = (queue **)malloc((b->m)*sizeof(queue *));
     for (i = 0; i< (b->m); i++) {
     (b->pointers)[i] = createQueue();
     }
     
     enqueue((b->pointers)[1], 0, 1);
     enqueue((b->pointers)[1], 1, 1);
     enqueue((b->pointers)[2], 2, 1);
     
     // test sur b
     printf("%ld\n", (b->pointers)[2]->first->j);
     
     // test produit
     smatrix *r = product(a, b, 1);
     printf("%ld\n", (r->pointers)[0]->first->j);
     printf("%ld\n", (r->pointers)[0]->first->v);
     printf("%ld\n", (r->pointers)[1]->first->j);
     printf("%ld\n", (r->pointers)[1]->first->v);
     
     // test display présmatrix
     displaySmatrix(a);
     displaySmatrix(b);
     displaySmatrix(r);
     */
free_matrixes:
    freeSmatrix(result);
    freeSmatrix(right);
    freeSmatrix(left);
    
close_ifile:
    /* fermeture du fichier d'entrée */
    if (fclose(ides) < 0) {
        perror("close(input_file)");
        exit_status = EXIT_FAILURE;
    }
    
end:
    return exit_status;
}

