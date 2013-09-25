#ifndef _queue_h
#define _queue_h


/* Noeud permettant de construire une file sur base d'une liste simplement chaînée. */
typedef struct node_t {
    int j;
    int v;
    struct node_t *next; /* Pointeur vers le noeud suivant */
} node;

/* File simplement chaînée. */
typedef struct queue_t {
    struct node_t *first; /* Pointeur vers le premier noeud */
    struct node_t *last;

} queue;

/* Crée une nouvelle file.
 *
 * Renvoie un pointeur vers la file. En cas d'erreur, retourne NULL. */
queue *createQueue();

/* Ajoute une image à la fin de la file.
 *
 * Si la fonction retourne 0, l'image a été ajoutée à la file avec succès. Si
 * la file, l'image ou la chaîne de caractères est NULL et en cas d'erreur, la
 * fonction renvoie une -1.
 */
int enqueue(queue *, int, int);


/* Libère la mémoire de la file entière.
 */
void freeQueue(queue *);

#endif

