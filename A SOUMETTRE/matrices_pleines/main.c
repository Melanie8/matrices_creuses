/*
 * Written by :
 * Lena Peschke <lena.peschke@student.uclouvain.be>
 * Mélanie Sedda <melanie.sedda@student.uclouvain.be>
 * October 2013
 */

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
#include "fmatrix.h"
#include "read.h"

int main(int argc, char *argv[]) {
    
    
    char *ifile = NULL; /* nom du fichier d'entrée */
    FILE *ides = NULL; /* descripteur de fichier associé au fichier d'entrée */
    char *ofile = NULL; /* nom du fichier de sortie */
    
    int exit_status = EXIT_SUCCESS; /* valeur de retour de la main */
    
    
    /*
     * Récupération des arguments
     */
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
    fmatrix *left; /* matrice pour prémultiplier */
    fmatrix *right; /* matrice pour postmultiplier */
    fmatrix *result; /* matrice de résultat */
    fmatrix *current; /* matrice en cours d'utilisation */
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
            left = create_fmatrix(snippet);
            if (!left) {
                fprintf(stderr, "Error calling create_fmatrix :"
                        " no fmatrix created\n");
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
            right = create_fmatrix(snippet);
            if (!right) {
                fprintf(stderr, "Error calling create_fmatrix :"
                        " no fmatrix created\n");
                exit_status = EXIT_FAILURE;
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                goto free_fmatrixes;
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
            result = fmatrix_product(left, right);
            if (!result) {
                fprintf(stderr, "Error calling fmatrix_product :"
                        " no fmatrix created\n");
                exit_status = EXIT_FAILURE;
                if (snippet) {
                    free(snippet); snippet = NULL;
                }
                goto free_fmatrixes;
            }
            /* libération de la mémoire des matrices qui ne sont plus utiles */
            free_fmatrix(left); left = NULL;
            free_fmatrix(right); right = NULL;
            
            /* stocke le résultat dans left pour la prochaine multiplication */
            left = result;
            result = NULL;
            available--;
        }
    }
    free(snippet); snippet = NULL;
    
   
    
//    long size = 100;
//    fmatrix *left = allocate_fmatrix(size, size);
//    left->n = size;
//    left->m = size;
//    long i, j;
//    for (i=0; i<size; i++) {
//        for (j=0; j<size; j++) {
//            if (j==i || j==((i+1)%100) || j==((i+2)%100)) {
//                (left->pointers)[i][j] = 1;
//            } else {
//                (left->pointers)[i][j] = 0;
//            }
//        }
//    }
    
    
    
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
    if (print_fmatrix(left, odes) != 0) {
        fprintf(stderr, "Error calling print_fmatrix\n");
        exit_status = EXIT_FAILURE;
        goto close_ofile;
    }
    
    /*
     * Terminaison du programme
     */
    
close_ofile:
    /* fermeture du fichier de sortie, s'il existe */
    if (ofile) {
        if (fclose(odes) < 0) {
            perror("close(output_file)");
            exit_status = EXIT_FAILURE;
        }
    }
    
free_fmatrixes:
    /* libération de la mémoire de la matrice */
    free_fmatrix(left);
    
close_ifile:
    /* fermeture du fichier d'entrée */
    if (fclose(ides) < 0) {
        perror("close(input_file)");
        exit_status = EXIT_FAILURE;
    }

end:
    return exit_status;
}
