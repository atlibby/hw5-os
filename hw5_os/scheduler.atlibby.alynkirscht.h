#ifndef HW5_OS_SCHEDULER_ATLIBBY_ALYNKIRSCHT_H
#define HW5_OS_SCHEDULER_ATLIBBY_ALYNKIRSCHT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "pqueue.h"

typedef struct {
    int taskID; // unique ID for this task
    int submitTime; // the time at which this task is submitted
    int totalBurstTime; // total time that this task will run
    int totalWaitTime; // the total time that this task has waited
    int remainingBurstTime; // the time that this task still needs; for RR
    int lastRunTime; // the last time this task ran; for RR
    int queueLength;
} Task;

int scheduler(int schedulerType, PQueueNode **taskQueue, PQueueNode **futureQueue, int quantum, int contextSwitchTime);
void createTask(PQueueNode **futureQueue, int taskID, int submitTime, int totalBurstTime);
int exprand(double mean);
void createRandomTasks(PQueueNode **futureQueue, Task **tasks, int n, double meanBurstTime, double meanInterarrivalTime);
int main();
#endif //HW5_OS_SCHEDULER_ATLIBBY_ALYNKIRSCHT_H
