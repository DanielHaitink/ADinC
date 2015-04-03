/*
//  main.c
//  Maze
//
//  Created by Daniël Haitink on 18/03/15.
//  Copyright (c) 2015 Daniël Haitink. All rights reserved.
*/

#include "maze.h"
#include "queues.h"

/* alloccates memory for a 2D char array */
char **makeCharArray(board s, char **array){
    int h;
    array=calloc(s.length, sizeof(char*));
    assert(array!=NULL);
    for (h=0; h<s.length; h++) {
        array[h]=calloc(s.width, sizeof(char));
        assert(array[h]!=NULL);
    }
    return array;
}

/* clears the flag array in m */
void clearArray(board *s, maze *m){
    int h, w;
    for (h=0; h<s->length; h++) {
        for (w=0; w<s->width; w++) {
            m->flag[h][w]=0;
        }
    }
}

/* prints the maze */
void printArray (board s, maze m) {
    int h, w;
    /* PRINT MAZE*/
    for (h=0; h<s.length; h++) {
        for (w=0; w<s.width; w++) {
            printf("%c", m.field[h][w]);
        }
        printf("\n");
    }
}

/* reads the userinput and stores it in s and m */
void readInput(board *s, maze *m){
    int j,h,w, currentPerson;
    char currentChar;
    scanf("%d", &s->gate);
    scanf("%d", &s->bed);
    
    /* ALLOC PEOPLE */
    scanf("%d", &s->numberOfPeople);
    
    /* SCAN PEOPLE */
    for (j=0; j<s->numberOfPeople; j++) {
        scanf("%d", &currentPerson);
        if (currentPerson > s->bed && currentPerson < s->gate) {
            s->p.both = 1;
        } if (currentPerson > s->bed) {
            s->p.bed = 1;
        } if (currentPerson < s->gate){
            s->p.gate = 1;
        }
    }
    
    /* ALLOC FIELD */
    scanf("%d", &s->length);
    scanf("%d", &s->width);
    m->field = makeCharArray(*s, m->field);
    m->flag = makeCharArray(*s, m->flag);
        
    /* SCAN FIELD */
    
    for (h=0; h < s->length; h++) {
        w=0;
        while (w < s->width) {
            currentChar = getchar();
            if (currentChar == '^' || currentChar == '#' || currentChar == '.' || currentChar == '%') {
                m->field[h][w] = currentChar;
                w++;
            }
        }
    }
    
    /* SET OTHER VARIABLES IN S */
    s->x = (s->width-1)/2;
    s->y = (s->length-1)/2;
    s->time = 0;
}

/* free the maze and the flag arrays */
void freeBoard(board *s, maze *m){
    int h;
    for (h=0; h<s->length; h++) {
        free(m->field[h]);
        free(m->flag[h]);
    }
    free(m->field);
    free(m->flag);
}

/* Check if a solution is found */
int checkCorrect(board s){
    if (s.x==0 || s.y==0 || s.x==s.width-1 || s.y==s.length-1){
        return 1;
    }
    return 0;
}

/* See if the new location is viable for the current persontype */
int canPass(char space, int person){
    switch (space) {
        case '.':
            return 1;
        case '%':
            if (person == BOTH || person == BED){
                return 1;
            }
            return 0;
        case '^':
            if (person == BOTH || person == GATE){
                return 1;
            }
            return 0;
    }
    return 0;
}

/* check if the wanted move is possible, if not return 0 */
int stepPossible(int x, int y, board s, int person, maze m){
    if (x >= s.width || x < 0) {
        return 0;
    }
    if (y >= s.length || y < 0) {
        return 0;
    }
    if (!canPass(m.field[y][x], person)) {
        return 0;
    }
    return 1;
}

/* returns a state in the direction desired */
board newPlace(board s, int direction){
    if (direction == NORTH) {
        s.y= s.y-1;
        s.time++;
    }
    
    /* RIGHT */
    if (direction == EAST) {
        s.x= s.x+1;
        s.time++;
    }
    
    /* DOWN */
    if (direction == SOUTH) {
        s.y= s.y+1;
        s.time++;
    }
    /* LEFT */
    if (direction == WEST) {
        s.x= s.x-1;
        s.time++;
    }
    return s;
}

/* use breadth first search to search through the maze */
int bfs(board s, maze m, int person){
    Queue q = newEmptyQueue();
    board s2;
    enqueue(s, &q);
    while (!isEmptyQueue(q)) {
        s = dequeue(&q);
        s2=s;
        m.flag[s.y][s.x] = 1;
        
        if (s.x==0 || s.y==0 || s.x==s.width-1 || s.y==s.length-1){
            /* if it is a faster time, it gets saved, else it gets ignored */

            freeQueue(q);
            return s.time+1;
        }
        
        /* Create max. 4 new states in which we move to all possible directions. Enqueue this */
        
        /* UP */
        if (stepPossible(s.x, s.y-1, s, person, m) && m.flag[s.y-1][s.x]==0) {
            s2 = newPlace(s, NORTH);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.time+1;
            }
            m.flag[s.y-1][s.x] = 1;
            enqueue(newPlace(s, NORTH), &q);
            s2=s;
        }
        
        /* RIGHT */
        if (stepPossible(s.x+1, s.y, s, person, m) && m.flag[s.y][s.x+1]==0) {

            s2 = newPlace(s, EAST);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.time+1;
            }
            m.flag[s.y][s.x+1] = 1;
            enqueue(newPlace(s, EAST), &q);
            s2=s;
        }
        
        /* DOWN */
        if (stepPossible(s.x, s.y+1, s, person, m) && m.flag[s.y+1][s.x]==0) {

            s2 = newPlace(s, SOUTH);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.time+1;
            }
            m.flag[s.y+1][s.x] = 1;
            enqueue(newPlace(s, SOUTH), &q);
            s2=s;
        }
        /* LEFT */
        if (stepPossible(s.x-1, s.y, s, person, m) && m.flag[s.y][s.x-1]==0) {

            s2 = newPlace(s, WEST);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.time+1;
            }
            m.flag[s.y][s.x-1] = 1;
            enqueue(newPlace(s, WEST), &q);
            s2=s;
        }
    }
    /* FREE Queue and return Max Int (Something went wrong) */
    freeQueue(q);
    return INT_MAX;
}

/* Set initial variables (x, y, time and clean flag array) */
void setInitial(board *s, maze *m){
    s->x= (s->width-1)/2;
    s->y= (s->length-1)/2;
    s->time = 0;
    clearArray(s, m);
}

/* Check if the board has a solution (NOT NEEDED) */
void isSol(board s, maze m){
    int h, w;
    for (h=0; h<s.length; h++) {
        if (m.field[h][0] != '#' || m.field[h][s.width-1] != '#') {
            printf("FOUND SOL %c OR %c\n", m.field[h][0], m.field[h][s.width-1]);
        }
    }
    
    for (w=0; w<s.width; w++) {
        if (m.field[0][w] != '#' || m.field[s.length-1][w] != '#') {
            printf("FOUND SOL %c OR %c\n", m.field[0][w], m.field[s.length-1][w]);
        }
    }
    
}

int main(int argc, const char * argv[]) {
    board s;
    maze m;
    int numberOfMazes, i, both = 0, gate = 0, bed = 0, none = 0, best = -1, current;
    
    /* scan the number of mazes */
    scanf("%d",&numberOfMazes);
    for (i=0; i<numberOfMazes; i++) {
        both = 0;
        gate = 0;
        bed = 0;
        none = 0;
        best = -1;
        s.p.bed=0;
        s.p.both=0;
        s.p.gate=0;
        
        /* read input into m and s */
        readInput(&s, &m);
        /* SET OTHER VARIABLES IN S */
        s.x = (s.width-1)/2;
        s.y = (s.length-1)/2;
        s.time = 0;
        
        /* Go into BFS with the correct Persontype */
        if (s.p.both == 1) {
            best = bfs(s, m, BOTH);
        } else {
            if (s.p.gate == 1) {
                best = bfs(s, m, GATE);
                setInitial(&s, &m);
            }
            if (s.p.bed == 1) {
                current = bfs(s,m, BED);

                if (best == -1 || current < best) {
                    best = current;
                }
            }
            if (s.p.bed != 1 && s.p.gate != 1) {
                best = bfs(s,m, NONE);
            }
        }
        /* Print the solution */
        printf("%d\n", best);
        
        /* Free the maze and flag */
        freeBoard(&s, &m);
    }
    
    return 0;
}
