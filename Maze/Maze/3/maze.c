/*
//  main.c
//  Maze
//
//  Created by Daniël Haitink on 18/03/15.
//  Copyright (c) 2015 Daniël Haitink. All rights reserved.
*/

#include "maze.h"
#include "queues.h"
int checkCorrect(board s){
    if (s.m.x==0 || s.m.y==0 || s.m.x==s.width-1 || s.m.y==s.length-1){
        return 1;
    }
    return 0;
}

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

void clearArray(board *s){
    int h, w;
    for (h=0; h<s->length; h++) {
        for (w=0; w<s->width; w++) {
            s->m.flag[h][w]=0;
        }
    }
}

void printArray (board s) {
    int h, w;
    /* PRINT MAZE*/
    for (h=0; h<s.length; h++) {
        for (w=0; w<s.width; w++) {
            printf("%c", s.m.field[h][w]);
        }
        printf("\n");
    }
}

void readInput(board *s){
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
    s->m.field = makeCharArray(*s, s->m.field);
    s->m.flag = makeCharArray(*s, s->m.flag);
        
    /* SCAN FIELD */
    for (h=0; h < s->length; h++) {
        w=0;
        while (w < s->width) {
            currentChar = getchar();
            if (currentChar == '^' || currentChar == '#' || currentChar == '.' || currentChar == '%') {
                s->m.field[h][w] = currentChar;
                w++;
            }
        }
    }
    
   /* for (h=0; h<s->length; h++) {
        for (w=0; w<s->width; w++) {
            do {
                currentChar = getchar();
            } while (currentChar == '\n');
            s->m.field[h][w] = currentChar;
            
        }
    }
    
    for (h = 0; h < s->length; h++) {
        char* line = malloc((s->width + 1) * sizeof (char));
        scanf("%s", line);
        for (w = 0; w < s->width; w++) {
            s->m.field[h][w] = line[w];
        }
        free(line);
    }
    */
    /* SET OTHER VARIABLES IN S */
    s->m.x = (s->width-1)/2;
    s->m.y = (s->length-1)/2;
    s->m.time = 0;
}

void freeBoard(board *s){
    int h;
    for (h=0; h<s->length; h++) {
        free(s->m.field[h]);
        free(s->m.flag[h]);
    }
    free(s->m.field);
    free(s->m.flag);
}

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

/* 1=N 2=E 3=S 4=W */
int stepPossible(int x, int y, board s, int person){
    if (x >= s.width || x < 0) {
        return 0;
    }
    if (y >= s.length || y < 0) {
        return 0;
    }
    if (!canPass(s.m.field[y][x], person)) {
        return 0;
    }
    return 1;
}

board newPlace(board s, int direction){
    if (direction == NORTH) {
        s.m.y= s.m.y-1;
        s.m.time++;
    }
    
    /* RIGHT */
    if (direction == EAST) {
        s.m.x= s.m.x+1;
        s.m.time++;
    }
    
    /* DOWN */
    if (direction == SOUTH) {
        s.m.y= s.m.y+1;
        s.m.time++;
    }
    /* LEFT */
    if (direction == WEST) {
        s.m.x= s.m.x-1;
        s.m.time++;
    }
    return s;
}

int bfs(board s, int person){
    Queue q = newEmptyQueue();
    board s2;
    enqueue(s, &q);
    while (!isEmptyQueue(q)) {
        s = dequeue(&q);
        s2=s;
        s.m.flag[s.m.y][s.m.x] = 1;
        
        if (s.m.x==0 || s.m.y==0 || s.m.x==s.width-1 || s.m.y==s.length-1){
            /* if it is a faster time, it gets saved, else it gets ignored */
            /*printf("FINISH %d %d %c\n", s.m.x, s.m.y, s.m.field[s.m.x][s.m.y]);*/

            freeQueue(q);
            /* FREE STATE!! */
            return s.m.time+1;
        }
        
        /* UP */
        if (stepPossible(s.m.x, s.m.y-1, s, person) && s.m.flag[s.m.y-1][s.m.x]==0) {
            s2 = newPlace(s, NORTH);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.m.time+1;
            }
            enqueue(newPlace(s, NORTH), &q);
            s2=s;
        }
        
        /* RIGHT */
        if (stepPossible(s.m.x+1, s.m.y, s, person) && s.m.flag[s.m.y][s.m.x+1]==0) {
            s2 = newPlace(s, EAST);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.m.time+1;
            }
            enqueue(newPlace(s, EAST), &q);
            s2=s;
        }
        
        /* DOWN */
        if (stepPossible(s.m.x, s.m.y+1, s, person) && s.m.flag[s.m.y+1][s.m.x]==0) {
            s2 = newPlace(s, SOUTH);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.m.time+1;
            }
            enqueue(newPlace(s, SOUTH), &q);
            s2=s;
        }
        /* LEFT */
        if (stepPossible(s.m.x-1, s.m.y, s, person) && s.m.flag[s.m.y][s.m.x-1]==0) {
            s2 = newPlace(s, WEST);
            if(checkCorrect(s2)){
                freeQueue(q);
                return s2.m.time+1;
            }
            enqueue(newPlace(s, WEST), &q);
            s2=s;
        }
    }
    /* FREE STATE */
    freeQueue(q);
    return INT_MAX;
}

void setInitial(board *s){
    s->m.x= (s->width-1)/2;
    s->m.y= (s->length-1)/2;
    s->m.time = 0;
    clearArray(s);
}

int main(int argc, const char * argv[]) {
    board s;
    int numberOfMazes, i, both = 0, gate = 0, bed = 0, none = 0, best = -1, current;
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
        
        /*readInput(&s);*/
        /************************************/
        int j,h,w, currentPerson;
        char currentChar;
        scanf("%d", &s.gate);
        scanf("%d", &s.bed);
        
        /* ALLOC PEOPLE */
        scanf("%d", &s.numberOfPeople);
        
        /* SCAN PEOPLE */
        for (j=0; j<s.numberOfPeople; j++) {
            scanf("%d", &currentPerson);
            if (currentPerson > s.bed && currentPerson < s.gate) {
                s.p.both = 1;
            } if (currentPerson > s.bed) {
                s.p.bed = 1;
            } if (currentPerson < s.gate){
                s.p.gate = 1;
            }
        }
        
        /* ALLOC FIELD */
        scanf("%d", &s.length);
        scanf("%d", &s.width);
        s.m.field = makeCharArray(s, s.m.field);
        s.m.flag = makeCharArray(s, s.m.flag);
        /* SCAN FIELD */
        
        for (h=0; h < s.length; h++) {
            w=0;
            while (w < s.width) {
                currentChar = getchar();
                if (currentChar == '^' || currentChar == '#' || currentChar == '.' || currentChar == '%') {
                    s.m.field[h][w] = currentChar;
                    w++;
                }
            }
        }
        
        /* for (h=0; h<s.length; h++) {
         for (w=0; w<s.width; w++) {
         do {
         currentChar = getchar();
         } while (currentChar == '\n');
         s.m.field[h][w] = currentChar;
         
         }
         }
         
         for (h = 0; h < s.length; h++) {
         char* line = malloc((s.width + 1) * sizeof (char));
         scanf("%s", line);
         for (w = 0; w < s.width; w++) {
         s.m.field[h][w] = line[w];
         }
         free(line);
         }
         */
        /* SET OTHER VARIABLES IN S */
        s.m.x = (s.width-1)/2;
        s.m.y = (s.length-1)/2;
        s.m.time = 0;
        
        /*******************************/
        
        if (s.p.both == 1) {
            current = bfs(s, BOTH);
           /* printf("BOTH %d\n", current);*/
            if (best == -1 || current < best) {
                best = current;
            }
            setInitial(&s);
        } else {
            if (s.p.gate == 1) {
                current = bfs(s, GATE);
                /*printf("GATE %d\n", current);*/

                if (best == -1 || current < best) {
                    best = current;
                }
                setInitial(&s);
            }
            if (s.p.bed == 1) {
                current = bfs(s, BED);
                /*printf("BED %d\n", current);*/

                if (best == -1 || current < best) {
                    best = current;
                }
                setInitial(&s);
            }
            if (s.p.bed != 1 && s.p.gate != 1) {
                current = bfs(s, NONE);
                /*printf("NONE %d\n", current);*/

                if (best == -1 || current < best) {
                    best = current;
                }
                setInitial(&s);
            }
        }
        printf("%d\n", best);
        /* FREE */

        freeBoard(&s);
    }
    
    return 0;
}
