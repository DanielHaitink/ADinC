/*
//  queue.c
//  
//
//  Created by Daniël Haitink on 20/02/15.
//
*/

#include "queues.h"
#include "sandglasses.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* We use the functions on lists as defined in 1.3 of the lecture notes.
 */


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

List addItem(State s, List li) {
    List newList = malloc(sizeof(struct ListNode));
    assert(newList!=NULL);
    newList->item = s;
    newList->next = li;
    return newList;
}

State firstItem(List li) {
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

void enqueue (State s, Queue *qp) {
    if ( isEmptyList(qp->list) ) {
        qp->list = addItem(s,NULL);
        qp->lastNode = qp->list;
    } else {
        (qp->lastNode)->next = addItem(s,NULL);
        qp->lastNode = (qp->lastNode->next);
    }
}

State dequeue(Queue *qp) {
    State s = firstItem(qp->list);
    qp->list = removeFirstNode(qp->list);
    if ( isEmptyList(qp->list) )  {
        qp->lastNode = NULL;
    }
    return s;
}

void freeQueue (Queue q) {
    freeList(q.list);
}

/****************************************************************************************************/
/* compare the times of the given States */
int compare (State a, State b){
    
    /* Time is Equal, check sandglasses */
    if (a.time==b.time) {
        if (a.sg1==b.sg1 && a.sg2 == b.sg2) {
            /* EXACT state is present, do nothing */
            return 0;
        }
        if (a.sg1 < b.sg2) {
            /* sg1 is smaller, still not a t the right place */
            return -1;
        }
        if (a.sg1 >= b.sg1) {
            /* Sg1 is bigger, almost at correct place */
            if (a.sg2 < b.sg2) {
                /* Sg2 is still smaller, not at the right place */
                return -1;
            }
            if (a.sg2 >= b.sg2) {
                /* Sg2 is bigger, at the right place */
                return 1;
            }
        }
    }
    if (a.time < b.time) {
        /* Time too low, not at the right place */
        return -1;
    }
    if (a.time > b.time) {
        /* Time is bigger, thus at the correct place */
        return 1;
    }
    return -99;
}

List InsrtUniq(State s, List l){
    /* If list is empty, create a new listNode and return it */
    if (isEmptyList(l)) {
        l = addItem(s,NULL);
        return l;
    }
    /* If compare is 0, return l. Exact State is already present */
    if (compare(s, l->item)==0) {
        return l;
    }
    /* if compare is 1, we create a new item with state s, which points to l */
    if (compare(s, l->item) == 1) {
        List l1 = addItem(s, l);
        return l1;
    }
    /* the new pointer to the next ListNode is the return value of InsrtUniq */
    l->next = (InsrtUniq(s, l->next));
    /* return current List */
    return l;
}

/* function as called by sandglasses.c. */
Queue* insertUniqueRec (State s, Queue *q){
    if (isEmptyList(q->list)) {
        q->list = addItem(s,NULL);
        q->lastNode = q->list;
        return q;
    }
    if (compare(s, q->list->item) == 0) {
        return q;
    }
    if (compare(s, q->list->item) == 1) {
        List l1 = addItem(s, q->list);
        q->list = l1;
        return q;
    }
    Queue q1;
    q1.list = q->list->next;
    q1.lastNode = q->lastNode;
    (q->list)->next = (insertUnique(s, &q1)->list);
    //freeQueue(q1);
    return q;
}


/****************************************************************************************************/