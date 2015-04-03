/*
//  queues.h
//  Maze
//
//  Created by Daniël Haitink on 18/03/15.
//  Copyright (c) 2015 Daniël Haitink. All rights reserved.
*/

#ifndef __Maze__queues__
#define __Maze__queues__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "maze.h"

/* struct for the maze and field */
typedef struct maze{
    char **field;
    char **flag;
}maze;

/* struct for the type of people available */
typedef struct people{
    int gate;
    int bed;
    int both;
}people;

/* struct of the needed information of a state */
typedef struct board{
    int length;
    int width;
    int gate;
    int bed;
    people p;
    int numberOfPeople;
    int time;
    int x;
    int y;
}board;

typedef struct ListNode *List;  /* List is the type of lists of states */

typedef struct ListNode {
    board item;
    List next;
}ListNode;

typedef struct Queue { /* a queue is a list and a pointer to the last node */
    List list;
    List lastNode;
} Queue;

Queue newEmptyQueue ();
int isEmptyQueue (Queue q);
void enqueue (board s, Queue *qp);
board dequeue(Queue *qp);
void freeQueue (Queue q);

#endif /* defined(__Maze__queues__) */
