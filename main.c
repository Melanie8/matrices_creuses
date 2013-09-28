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
//#include <pthread.h>
//#include <semaphore.h>
#include "smatrix.h"
#include "queue.h"

int main(int argc, char *argv[]) {
    
    //int i; // variable d'itération
    //int err; // variable d'erreur
    
    char *ifile = NULL; // nom du fichier d'entrée
    int ides; // descripteur de fichier associé au fichier d'entrée
    
    char *ofile = NULL;
    
    /* arguments */
    // attention, -o doit toujours venir en premier!!
    // vérifier le nombre d'arguments?
    int optch;
    while ((optch = getopt(argc, argv, "o:")) != -1)
        switch (optch) {
            case 'o':
                printf ("Paramètre o rencontré avec argument %s\n", optarg);
                ofile = optarg;
                printf("ofile: %s\n", ofile);
                break;
            default:
                printf("da\n");
                fprintf(stderr, "Usage: %s [-o output_file] input_file\n", argv[0]);
                return EXIT_FAILURE;
        }
    
    ifile = argv[optind];
    printf("infile: %s\n", ifile);
    
    
    /* ouverture du fichier infile */
    if ((ides = open(ifile, O_RDONLY)) < 0) {
        perror("open(input_file)");
        return EXIT_FAILURE;
    }
    /* pas encore utile
    struct stat istat;
    if ((fstat(ides, &istat)) < 0) {
        perror("error occurred: fstat\n");
        close(ides); // vérifie pas la valeur de retour car déjà en erreur
        return EXIT_FAILURE;
    }
    int isize = (size_t) istat.st_size;
     */
    
    // a garder
    /*
    smatrix left;
    smatrix rigth;
    smatrix result;
     */
    
    // lecture de fichier
    
    /* fermeture du fichier d'entrée */
    if (close(ides) < 0) {
        perror("close(input_file)");
        return EXIT_FAILURE;
    }
    
    
    
    /* SECTION TEST */
    
    char *s = "-1x4";
    char *t = "2x3";
    
    smatrix *matrixdim1 = createSmatrix(s, true);
    smatrix *matrixdim2 = createSmatrix(t, true);
    
    printf("le free commence\n");
    freeSmatrix(matrixdim1);
    freeSmatrix(matrixdim2);
    
    /*
    // matrice creuse a
    smatrix *a = (smatrix *)malloc(sizeof(smatrix));
    if (a==NULL)
        return(EXIT_FAILURE);
    
    a->n = 2;
    a->m = 3;
    a->pointers = (queue **)malloc((a->n)*sizeof(queue *));
    //int i;
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
    */
    
    return (EXIT_SUCCESS);
}

