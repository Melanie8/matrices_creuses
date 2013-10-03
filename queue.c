#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

queue *create_queue() {
    /* Alloue de la mémoire pour la nouvelle file */
    queue *q = (queue *)calloc(1, sizeof(queue));
    if (!q) {
        fprintf(stderr, "Error calling create_queue : calloc to initialise a"
                " queue failed");
        return NULL;
    }
    q->first = NULL;
    q->last = NULL;
    
    /* Renvoie la nouvelle file */
    return q;
}

int enqueue(queue *q, long j, long v) {
    /* Vérifie que q est non NULL */
    if (!q) {
		fprintf(stderr, "Error calling enqueue : the queue is NULL \n");
        return -1;
    }
    
    /* Alloue de la mémoire pour le nouveau noeud */
    node *new = (node *)calloc(1, sizeof(node));
    if (!new) {
        fprintf(stderr, "Error calling enqueue : calloc to create a"
                " new node failed\n");
        return -1;
    }
	
	/* Enregistre j et v dans le noeud */
	new->j = j;
    new->v = v;
    
    /* Modifie les liens de la file. Si la file est vide, il faut aussi
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

void free_queue(queue *q) {
    /* Vérifie que q est non NULL. */
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

