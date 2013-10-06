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
    long size = 1600;
    long number = 0.07*size;
    fmatrix *left = allocate_fmatrix(size, size);
    left->n = size;
    left->m = size;
    long i, j;
    for (i=0; i<size; i++) {
        for (j=0; j<size; j++) {
            if ((0<=(j-i) && (j-i)<number) || ((j+number)>=size && (i-j)>size)) {
                (left->pointers)[i][j] = 1;
            } else {
                (left->pointers)[i][j] = 0;
            }
        }
    }
    
    
    
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
