//
// Created by Andrew Libby on 11/8/23.
//

#ifndef HW5_OS_SCHEDULER_ATLIBBY_ALYNKIRSCHT_H
#define HW5_OS_SCHEDULER_ATLIBBY_ALYNKIRSCHT_H

typedef struct {
    int taskID; // unique ID for this task
    int submitTime; // the time at which this task is submitted
    int totalBurstTime; // total time that this task will run
    int totalWaitTime; // the total time that this task has waited
} Task;

typedef struct SchedulerNodeStruct {
    int priority;
    void *data;
    struct SchedulerNodeStruct *next;
} SchedulerNode;

int enqueue(SchedulerNode **scheduler, int priority, void *data);
void *dequeue(SchedulerNode **scheduler);
void *peek(SchedulerNode *scheduler);
void printScheduler(SchedulerNode *scheduler, void (printFunction)(void*));
int getMinPriority(SchedulerNode *scheduler);
int schedulerLength(SchedulerNode *scheduler);

void printTask(void *data);

#endif //HW5_OS_SCHEDULER_ATLIBBY_ALYNKIRSCHT_H
