/*
//  queue.h
//  
//
//  Created by Daniël Haitink on 20/02/15.
//
*/

#ifndef ____queue__
#define ____queue__

#include <stdio.h>

/* First the type definitions.
 */

typedef struct State { /* a state contains a time and the states of the two sandglasses */
    int time;
    int sg1, sg2; /* amount in upper part of sandglass */
} State;

typedef struct ListNode *List;  /* List is the type of lists of states */

struct ListNode {
    State item;
    List next;
};

typedef struct Queue { /* a queue is a list and a pointer to the last node */
    List list;
    List lastNode;
} Queue;

Queue newEmptyQueue ();
int isEmptyQueue (Queue q);
void enqueue (State s, Queue *qp);
State dequeue(Queue *qp);
void freeQueue (Queue q);
Queue* insertUnique(State s, Queue *q);

#endif /* defined(____queue__) */
