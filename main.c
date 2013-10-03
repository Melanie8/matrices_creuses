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
#include "read.h"

int main(int argc, char *argv[]) {
    
    printf(" \n\n$$$$$$$$$$$$$ DEBUT DE CHEZ DEBUT $$$$$$$$$$$$$$$ \n\n");
    
    char *ifile = NULL; /* nom du fichier d'entrée */
    FILE *ides = NULL; /* descripteur de fichier associé au fichier d'entrée */
    char *ofile = NULL; /* nom du fichier de sortie */
    
    int exit_status = EXIT_SUCCESS; /* valeur de retour de la main */
    
    
    /* Récupération des arguments */
    if (argc < 2) {
        fprintf(stderr, "Error : too few arguments\n");
        exit_status = EXIT_FAILURE;
        goto end;
    } else if (argc > 4) {
        fprintf(stderr, "Error : too many arguments\n");
        exit_status = EXIT_FAILURE;
        goto end;
    }
    int optch;
    while ((optch = getopt(argc, argv, "o:")) != -1)
        switch (optch) {
            case 'o':
                ofile = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o output_file] input_file\n",
                        argv[0]);
                exit_status = EXIT_FAILURE;
                goto end;
        }
    
    ifile = argv[optind];
    
    /* 
     * Lecture du fichier et manipulation des matrices
     */
    
    /* Ouverture du fichier infile */
    if ((ides = fopen(ifile, "r")) < 0) {
        perror("fopen(input_file)");
        exit_status = EXIT_FAILURE;
        goto end;
    }

    /* Matrices utilisées pour les opérations */
    smatrix *left; /* matrice pour prémultiplier */
    smatrix *right; /* matrice pour postmultiplier */
    smatrix *result; /* matrice de résultat */
    smatrix *current; /* matrice en cours d'utilisation */
    bool left_already = false; /* indique si une matrice gauche existe */
    unsigned int available = 0; /* nombre de matrice en mémoire */
    
    /* Initialisation */
	ssize_t read;
	size_t len = 0;
	char *snippet = NULL;
    
    /* Boucle sur les lignes du fichier pour créer des matrices */
	while ((read = getline(&snippet, &len, ides)) > 0) {
        
        /* Création d'une nouvelle matrice */
        if (!left_already) {
            /* matrice gauche */
            left = create_smatrix(snippet, true);
            if (!left) {
                fprintf(stderr, "Error calling create_smatrix :"
                        " no matrix created\n");
                exit_status = EXIT_FAILURE;
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                goto close_ifile;
            }
            left_already = true;
            current = left;
            available++;
        } else {
            /* matrice droite */
            right = create_smatrix(snippet, false);
            if (!right) {
                fprintf(stderr, "Error calling create_smatrix :"
                        " no matrix created\n");
                exit_status = EXIT_FAILURE;
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                goto free_matrixes;
            }
            current = right;
            available++;
        }
        
        /* Remplissage de la matrice */
        int err = convert_lines(current, ides);
        if (err == -1) {
            fprintf(stderr, "Error calling convert_lines\n");
            if (snippet) {
                free(snippet); snippet = NULL;
            }
            exit_status = EXIT_FAILURE;
        }
        if (err != ((current->n)*(current->m))) {
            fprintf(stderr, "Error calling convert_lines: not enough"
                    " entries found in the file\n");
            if (snippet) {
                free(snippet); snippet = NULL;
            }
            exit_status = EXIT_FAILURE;
        }
        
        /* Multiplication de deux matrices */
        if (available == 2) {
            result = smatrix_product(left, right, true);
            if (!result) {
                fprintf(stderr, "Error calling smatrix_product :"
                        " no matrix created\n");
                exit_status = EXIT_FAILURE;
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                goto free_matrixes;
            }
            /* libération de la mémoire des matrices qui ne sont plus utiles */
            free_smatrix(left); left = NULL;
            free_smatrix(right); right = NULL;
            left = result; // stocke le résultat de la multiplication dans left
            result = NULL;
            left_already = true; // inutile
            available--;
        }
    }
    free(snippet); snippet = NULL;
    
    /*
     * Ecriture du résultat final
     */
    
    /* Descripteur de fichier pour l'écriture */
    FILE *odes;
    if (ofile) {
        /* soit il y a un fichier... */
        odes = fopen(ofile, "w");
    } else {
        /* soit on écrit sur la sortie std */
        odes = stdout;
    }
    
    /* Ecriture dans le fichier ou sur la sortie std */
    if (print_smatrix(left, odes) != 0) {
        fprintf(stderr, "Error calling print_smatrix\n");
        exit_status = EXIT_FAILURE;
        goto close_ofile;
    }
    
    // ENLEVER
    /* SECTION TEST */
    /*
     char *s = "-1x4";
     char *t = "2x3";
     
     smatrix *matrixdim1 = create_smatrix(s, true);
     smatrix *matrixdim2 = create_smatrix(t, true);
     
     printf("le free commence\n");
     free_smatrix(matrixdim1);
     free_smatrix(matrixdim2);
     
     
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
     smatrix *r = smatrix_product(a, b, 1);
     printf("%ld\n", (r->pointers)[0]->first->j);
     printf("%ld\n", (r->pointers)[0]->first->v);
     printf("%ld\n", (r->pointers)[1]->first->j);
     printf("%ld\n", (r->pointers)[1]->first->v);
     
     // test display présmatrix
     display_smatrix(a);
     display_smatrix(b);
     display_smatrix(r);
     */
    
close_ofile:
    /* fermeture du fichier de sortie, s'il existe */
    if (ofile) {
        if (fclose(odes) < 0) {
            perror("close(output_file)");
            exit_status = EXIT_FAILURE;
        }
    }
    
free_matrixes:
    /* libération de la mémoire des matrices */
    free_smatrix(result); // juste pour être sûr
    free_smatrix(right); // juste pour être sûr
    free_smatrix(left);
    printf("Si tu arrives ici, c'est bien :)\n"); // ENLEVER
    
close_ifile:
    /* fermeture du fichier d'entrée */
    if (fclose(ides) < 0) {
        perror("close(input_file)");
        exit_status = EXIT_FAILURE;
    }
    
end:
    return exit_status;
}
