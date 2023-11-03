//
// Created by Andrew Libby on 11/3/23.
//

#ifndef HW5_OS_PQUEUE_ATLIBBY_ALYNKIRSCHT_H
#define HW5_OS_PQUEUE_ATLIBBY_ALYNKIRSCHT_H

#define MAX_NAME_LENGTH 63

typedef struct {
    unsigned int weight;
    char name[1 + MAX_NAME_LENGTH];
} Thing;

typedef struct PQueueNodeStruct {
    int priority;
    void *data;
    struct PQueueNodeStruct *next;
} PQueueNode;

int enqueue(PQueueNode **pqueue, int priority, void *data);
void *dequeue(PQueueNode **pqueue);
void *peek(PQueueNode *pqueue);
void printQueue(PQueueNode *pqueue, void (printFunction)(void*));
int getMinPriority(PQueueNode *pqueue);
int queueLength(PQueueNode *pqueue);

void printThing(void *data);


#endif //HW5_OS_PQUEUE_ATLIBBY_ALYNKIRSCHT_H
