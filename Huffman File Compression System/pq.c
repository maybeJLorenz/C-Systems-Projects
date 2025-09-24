/*
 *
 *	pq.c file
 *
 */

#include "pq.h"

#include "node.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};

struct PriorityQueue {
    ListElement *front;
    ListElement *back;
    int size;
};

ListElement *listElm_create(Node *tree) {

    ListElement *new_elm = (ListElement *) malloc(sizeof(ListElement));

    if (!new_elm) {
        return NULL; // allocation failed
    }

    new_elm->tree = tree;
    new_elm->next = NULL;
    return new_elm;
}

// ( done ) - ready to be checked
PriorityQueue *pq_create(void) {

    PriorityQueue *pq = (PriorityQueue *) calloc(1, sizeof(PriorityQueue));
    if (!pq) {
        return NULL; // allocation failed
    }
    pq->front = NULL;
    pq->back = NULL;
    pq->size = 0;
    return pq;
}

// ( done ) - ready to be checked
void pq_free(PriorityQueue **q) {

    free(*q);
    *q = NULL;
}

// ( done ) - ready to be checked
bool pq_is_empty(PriorityQueue *q) {

    return (q == NULL || q->front == NULL);
}

// ( done ) - ready to be checked
bool pq_size_is_1(PriorityQueue *q) {

    return (q && q->size == 1);
}

// ( done ) - ready to be checked
void enqueue(PriorityQueue *q, Node *tree) {

    ListElement *listElm = listElm_create(tree);
    if (!listElm) {
        return; // return if tree is empty
    }

    listElm->tree = tree;
    listElm->next = NULL;

    if (pq_is_empty(q) || tree->weight < q->front->tree->weight) {
        listElm->next = q->front;
        q->front = listElm;
    } else {
        ListElement *curr = q->front;
        while (curr->next != NULL && curr->next->tree->weight <= tree->weight) {
            curr = curr->next;
        }
        listElm->next = curr->next;
        curr->next = listElm;
    }

    if (q->back == NULL) {
        q->back = listElm;
    }

    q->size++;
}

// ( done ) - ready tto be checked
Node *dequeue(PriorityQueue *q) {

    if (pq_is_empty(q)) {
        return NULL;
    }

    ListElement *elm = q->front;
    Node *tree = elm->tree;

    q->front = elm->next;
    free(elm);
    q->size--;

    if (q->front == NULL) {
        q->back = NULL;
    }

    return tree;
}

// ( done ) - ready to be checked
void pq_print(PriorityQueue *q) {

    if (pq_is_empty(q)) {
        printf("Priority Queue is empty.\n");
        return;
    }

    ListElement *curr = q->front;
    printf("Priority Queue:\n");

    while (curr != NULL) {
        node_print_tree(curr->tree);
        curr = curr->next; // moves to the next node
    }
}
