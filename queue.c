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
    q->first = NULL;
    q->last = NULL;
    /* On renvoie la nouvelle file. */
    return q;
}

int enqueue(queue *q, long j, long v) {
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
    
    return 0;
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
			free(temp); temp = NULL;
        }
    }
	free(q);
}

