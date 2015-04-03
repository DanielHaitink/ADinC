

#include "sandglasses.h"
#include "queues.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* The function action generates a new state from an existing state.
 */


/* action can perform three actions: turn sg1(act == 0), turn sg2(act == 1) or turn both(act == 2). This depends on the act value. It returns the changed state  */
State action(State s, int act, int cap1, int cap2) {
    switch (act) {
        case 0:
            s.sg1 = cap1 - s.sg1;
            return s;
        case 1:  /* left */
            s.sg2 = cap2 - s.sg2;
            return s;
        case 2:  /* right */
            s.sg1 = cap1 - s.sg1;
            s.sg2 = cap2 - s.sg2;
            return s;
    }
    return s;
}

/* The function timeable checks whether a given time can be determined
 * exactly by two sandglasses with given capacities.
 */

State addMaxTime(State s){
    if (s.sg1 < s.sg2) {
        s.time+=s.sg1;
        s.sg2 -= s.sg1;
        s.sg1 = 0;
        return s;
    }
    if (s.sg1 > s.sg2) {
        s.time+=s.sg2;
        s.sg1 -= s.sg2;
        s.sg2 = 0;
        return s;
    }
    /* EQUAL */
    s.time+=s.sg1;
    s.sg1=0;
    s.sg2=0;
    return s;
    
}

/* iterative timeable function  */
int timeable(int cap1, int cap2, int goalTime) {
    Queue q = newEmptyQueue();
    int change = 1;
    State s;
    s.time = 0;
    s.sg1=0;
    s.sg2 = 0;
    insertUnique(s, &q);
    
    if (cap1 > goalTime && cap2 > goalTime && goalTime != 0) {
        freeQueue(q);
        return 0;
    }
    if (cap1 == 1 || cap2 == 1) {
        freeQueue(q);
        return 1;
    }
    
    /* keep dequeueing untill queue is empty */
    while (!isEmptyQueue(q)) {
        s=dequeue(&q);
       /* printf("sg1:%d sg2:%d time:%d\n", s.sg1, s.sg2, s.time);*/
        if (s.time<=goalTime && s.sg1!=0 && s.sg2!=0) {
            if ( s.sg1+s.time==goalTime || s.sg2+s.time==goalTime || goalTime==0) {
                freeQueue(q);
                return 1;
            }
            s = addMaxTime(s);
            change = 1;
        }
        if (change == 1) {
            insertUnique(action(s, 0, cap1, cap2), &q);
            insertUnique(action(s, 1, cap1, cap2), &q);
            insertUnique(action(s, 2, cap1, cap2), &q);
        }
        change =0;
    }
    /* queue empty, return 0 */
    freeQueue(q);
    return 0;
}


/* main performs a dialogue with the user. The user is asked to provide three numbers:
 * the (positive) capacities of the sandglasses and the goal time (>= 0).
 * The program indicates whether the goal time can be determined exactly.
 * The dialogue is ended by the input '0'.
 */

int main(int argc, char *argv[]){
    int cap1, cap2, goalTime;
    
    printf("give the sandglass capacities and the goal time: ");
    scanf("%d",&cap1);
    while ( cap1 > 0 ) {
        scanf("%d",&cap2);
        assert( cap2 > 0 );
        scanf("%d",&goalTime);
        assert( goalTime >= 0 );
        
        if ( timeable(cap1,cap2,goalTime) ) {
            printf("%d and %d can time %d\n", cap1, cap2, goalTime);
        } else {
            printf("%d and %d cannot time %d\n", cap1, cap2, goalTime);
        }
        
        printf("\ngive the sandglass capacities and the goal time: ");
        scanf("%d",&cap1);
    }
    printf("good bye\n");
    return 0;
}