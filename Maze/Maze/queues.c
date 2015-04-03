/*
//  queues.c
//  Maze
//
//  Created by Daniël Haitink on 18/03/15.
//  Copyright (c) 2015 Daniël Haitink. All rights reserved.
*/

#include "queues.h"
#include "maze.h"


List newEmptyList() {
    return NULL;
}

int isEmptyList (List li) {
    return ( li==NULL );
}

void listEmptyError() {
    printf("list empty\n");
    abort();
}

List addItem(board s, List li) {
    List newList = malloc(sizeof(struct ListNode));
    assert(newList!=NULL);
    newList->item = s;
    newList->next = li;
    return newList;
}

board firstItem(List li) {
    if ( li == NULL ) {
        listEmptyError();
    }
    return li->item;
}

List removeFirstNode(List li) {
    List returnList;
    if ( li == NULL ) {
        listEmptyError();
    }
    returnList = li->next;
    free(li);
    return returnList;
}

void freeList(List li) {
    List li1;
    while ( li != NULL ) {
        li1 = li->next;
        free(li);
        li = li1;
    }
    return;
}

/* We define some functions on queues, based on the definitions in 1.3.1 of the
 * lecture notes. Integers are replaced by states, and enqueue has output type void here.
 */

Queue newEmptyQueue () {
    Queue q;
    q.list = newEmptyList();
    q.lastNode = NULL;
    return q;
}

int isEmptyQueue (Queue q) {
    return isEmptyList(q.list);
}

void emptyQueueError () {
    printf("queue empty\n");
    exit(0);
}

void enqueue (board s, Queue *qp) {
    if ( isEmptyList(qp->list) ) {
        qp->list = addItem(s,NULL);
        qp->lastNode = qp->list;
    } else {
        (qp->lastNode)->next = addItem(s,NULL);
        qp->lastNode = (qp->lastNode->next);
    }
}

board dequeue(Queue *qp) {
    board s = firstItem(qp->list);
    qp->list = removeFirstNode(qp->list);
    if ( isEmptyList(qp->list) )  {
        qp->lastNode = NULL;
    }
    return s;
}

void freeQueue (Queue q) {
    freeList(q.list);
}

