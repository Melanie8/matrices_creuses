#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

queue *createQueue() {
    /* On alloue de la mémoire pour la nouvelle file. */
    queue *q = (queue *)calloc(1, sizeof(queue));
    if (!q) {
        perror("Error calling calloc to initialise a queue");
        return NULL;
    }

    /* On renvoie la nouvelle file. */
    return q;
}

int enqueue(queue *q, int j, int v) {
    /* On vérifie que q, i et string sont non NULL. */
    if (!q) {
		fprintf(stderr, "Error calling enqueue : NULL arguments \n");
        return -1;
    }
    
    /* On alloue de la mémoire pour le nouveau noeud. */
    node *new = (node *)calloc(1, sizeof(node));
    if (!new) {
        perror("Error calling calloc to create a new node");
        return -1;
    }
	
	/* On enregistre j et v dans le noeud */
	new->j = j;
    new->v = v;
    
    /* On modifie les liens de la file. Si la file est vide, il faut aussi
	 * modifier 'first', sinon seul 'last' change. */
    if (!(q->first)) {
        q->first = new;
        q->last = new;
    } else {
        q->last->next = new;
        q->last = new;
    }
    
    /* Un nouvel élément a été ajouté à la file. */
    (q->count)++;
    
    return 0;
}	

int putNode(queue *q, struct node_t *n) {
	/* On vérifie que q et n sont non NULL. */
	if ((!q) || (!n)) {
		return -1;
	}

	/* On modifie les liens de la file. Si la file est vide, il faut aussi
	 * modifier 'first', sinon seul 'last change'. */
    if (!(q->first)) {
        q->first = n;
        q->last = n;
    } else {
        q->last->next = n;
        q->last = n;
    }
    
    /* Un nouvel élément a été ajouté à la file. */
    (q->count)++;
    
    return 0;
}

node *getNode(queue *q) {
    /* On vérifie que q est non NULL et non vide. */
    if ((!q) || !(q->first)) {
        return NULL;
    }

    /* Sinon, on retient le premier élément. */
    node *ret = q->first;
    
    /* Si la file contient un seul élément, il faut aussi modifier le pointeur
	 * 'last' de q. Sinon, seul 'first' est modifié et pointe sur l'élément
	 * suivant. */
    if ((q->first) == (q->last)) {
        q->first = NULL;
        q->last = NULL;
    } else {
        q->first = q->first->next;
    }
    
    /* On renvoie le noeud. */
    return ret;
}

void freeNode(node *n) {
	/* On vérifie que n est non NULL. */
	if (!n) {
        return;
    }
	
	/* Sinon, on libère l'espace de son attribut name et le sien. */
	free(n->img->pixels); n->img->pixels = NULL;
	free(n->img); n->img = NULL;
	free(n->name); n->name = NULL;
	free(n); n = NULL;
}

void freeQueue(queue *q) {
    /* On vérifie que q est non NULL. */
    if (!q) {
        return;
	/* Si q est vide, il faut juste libérer la file. */
	} else if(!(q->first)) {
    /* Sinon, il faut aussi libérer la mémoire de tous les noeuds avant de
	 * libérer celle de la file. */
    } else {
        node *temp;
        while (q->first) {
            temp = q->first;
            q->first = q->first->next;
			freeNode(temp); temp = NULL;
        }
    }
	free(q); q = NULL;
}

