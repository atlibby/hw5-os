//
// Created by Andrew Libby on 11/8/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.atlibby.alynkirscht.h"

// First come first serve (FCFS) and shortest job first (SJF) scheduling algorithm
// This function takes in a queue of tasks and schedules them using the FCFS and SJF algorithms
// The function returns a queue of tasks that have been scheduled in the order that they were scheduled
// The function also returns the total wait time for all tasks that were scheduled


int enqueue(SchedulerNode **scheduler, int priority, void *data) {
    SchedulerNode *newNode, *currNode, *prevNode;
    newNode = (SchedulerNode *) malloc(sizeof(SchedulerNode));
    newNode->next = NULL;
    newNode->data = data;
    newNode->priority = priority;

    if (*scheduler == NULL) {
        *scheduler = newNode;
    } else {
        prevNode = NULL;
        currNode = *scheduler;
        while (currNode != NULL && priority >= currNode->priority) {
            prevNode = currNode;
            currNode = currNode->next;
        }

        if (currNode == NULL) {
            // insert at end
            prevNode->next = newNode;
        } else {
            if (prevNode != NULL) {
                // insert in middle
                prevNode->next = newNode;
                newNode->next = currNode;
            } else {
                // insert at front
                newNode->next = currNode;
                *scheduler = newNode;
            }
        }
    }
    return(0);
}

void printScheduler(SchedulerNode *scheduler, void (printFunction)(void *) ) {
    SchedulerNode *currNode;
    currNode = scheduler;
    while (currNode != NULL) {
        printf("priority = %d", currNode->priority);
        printf(" data = ");
        (*printFunction)(currNode->data);
        currNode = currNode->next;
    }
}

int schedulerLength(SchedulerNode *scheduler) {
    SchedulerNode *currNode;
    int length = 0;
    currNode = scheduler;
    while (currNode != NULL) {
        length++;
        currNode = currNode->next;
    }
    return length;
}

void *dequeue(SchedulerNode **scheduler) {
    SchedulerNode *currNode;
    void *data;
    if (*scheduler == NULL) {
        return NULL;
    } else {
        currNode = *scheduler;
        *scheduler = currNode->next;
        data = currNode->data;
        free(currNode);
        return data;
    }
}

void *peek(SchedulerNode *scheduler) {
    if (scheduler == NULL)
        return(NULL);
    else
        return(scheduler->data);
}

int getMinPriority(SchedulerNode *scheduler) {
    if (scheduler == NULL)
        return(-1);
    else
        return(scheduler->priority);
}

void printTask(void *data) {
    Task *task;
    task = (Task *) data;
    printf("Task ID: %d\n", task->taskID);
    printf("Submit Time: %d\n", task->submitTime);
    printf("Total Burst Time: %d\n", task->totalBurstTime);
    printf("Total Wait Time: %d\n", task->totalWaitTime);
    printf("\n");
}
