/*
 * Written by :
 * Lena Peschke <lena.peschke@student.uclouvain.be>
 * Mélanie Sedda <melanie.sedda@student.uclouvain.be>
 * October 2013
 */

#ifndef _queue_h
#define _queue_h

/* Noeud permettant de construire une file sur base d'une liste
 * simplement chaînée. */
typedef struct node_t {
    long j;
    long v;
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
queue *create_queue();

/* Ajoute un noeud à la fin de la file.
 *
 * Si la fonction retourne 0, le noeud a été ajouté à la file avec succès.
 * Si la file, le noeud ou la chaîne de caractères est NULL et en cas 
 * d'erreur, la fonction renvoie -1.
 */
int enqueue(queue *, long, long);


/* Libère la mémoire de la file entière.
 */
void free_queue(queue *);

#endif
