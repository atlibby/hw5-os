#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scheduler.atlibby.alynkirscht.h"

#define MAX_PRIORITY 1000000

#define QUANTUM 3

// Create tasks and put them in the taskQueue
Task *createTask(int taskID, int submitTime, int totalBurstTime) {
    Task *task;
    if (taskID < 0)
        return NULL;
    if (submitTime < 0)
        return NULL;
    if (totalBurstTime < 0)
        return NULL;
    task = (Task *) malloc(sizeof(Task));
    task->taskID = taskID;
    task->submitTime = submitTime;
    task->totalBurstTime = totalBurstTime;
    task->totalWaitTime = 0;
    return task;
}

int schedulerTests(int algo_switch) {
    SchedulerNode *taskQueue = NULL;
    SchedulerNode *futureQueue = NULL;
    Task *task;

// First come, first served (FCFS) scheduling algorithm
if (algo_switch == 0) {
    printf("First come first served (FCFS) scheduling algorithm\n");
    taskQueue = NULL;
    futureQueue = NULL;
} else if (algo_switch == 1){
    printf("Shortest job first (SJF) scheduling algorithm\n");
    taskQueue = NULL;
    futureQueue = NULL;
}
else {
    printf("Round Robin (RR) scheduling algorithm\n");
    taskQueue = NULL;
    futureQueue = NULL;
}

    int currentTime = 0;

    int totalWaitTime = 0;

    int currentID = 1;

    Task *task1 = createTask(1, 0, 6);
    task1->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task1->submitTime, task1);

    Task *task2 = createTask(1, 2, 8);
    task2->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task2->submitTime, task2);

    Task *task3 = createTask(1, 4, 7);
    task3->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task3->submitTime, task3);

    Task *task4 = createTask(1, 8, 3);
    task4->taskID = currentID;

    enqueue(&futureQueue, task4->submitTime, task4);

    bool done = false;

    while (!done) {
        while (peek(futureQueue) != NULL || peek(taskQueue) != NULL) {
            while (peek(futureQueue) != NULL && futureQueue->priority <= currentTime) {
                printf("Time %d: Adding task %d to scheduler\n", currentTime, ((Task *) peek(futureQueue))->taskID);
                task = (Task *)dequeue(&futureQueue);
                if (algo_switch == 0) {
                    enqueue(&taskQueue, task->submitTime, task);
                } else {
                    enqueue(&taskQueue, task->totalBurstTime, task);
                }
            }

            if (peek(taskQueue) != NULL) {
                Task *runningTask = (Task *)dequeue(&taskQueue);

                if (algo_switch == 2) {
                    printf("Time %d: Task %d is running\n", currentTime, runningTask->taskID);
                    if (runningTask->lastRunTime > 0){
                        runningTask->totalWaitTime += (currentTime - runningTask->lastRunTime);
                    } else {
                        runningTask->totalWaitTime = (currentTime - runningTask->submitTime);
                    }

                    if (QUANTUM >= runningTask->remainingBurstTime){
                        currentTime += runningTask->remainingBurstTime;
                        totalWaitTime += runningTask->totalWaitTime;
                        printf("Time %d: Task %d finished\n", currentTime, runningTask->taskID);
                    }

                    else {
                        currentTime += QUANTUM;
                        runningTask->remainingBurstTime -= QUANTUM;
                        runningTask->lastRunTime = currentTime;
                        enqueue(&taskQueue, MAX_PRIORITY, runningTask);
                    }
                }

                else{
                    printf("Time %d: Task %d is running\n", currentTime, runningTask->taskID);
                    runningTask->totalWaitTime += currentTime - runningTask->submitTime;
                    totalWaitTime += runningTask->totalWaitTime;
                    currentTime += runningTask->totalBurstTime;
                    printf("Time %d: Task %d finished\n", currentTime, runningTask->taskID);
                }

            } else {
                currentTime = getMinPriority(futureQueue);
            }
        }
        if (taskQueue == NULL) {
            printf("Scheduler is empty\n");
            done = true;
            if (algo_switch == 0) {
                printf("FCFS total wait time: %d\n", totalWaitTime);
            } else if (algo_switch == 1)
                printf("SJF total wait time: %d\n", totalWaitTime);
            else {
                printf("Round Robin total wait time: %d\n", totalWaitTime);
            }
        }
    }
    return algo_switch;
}

int main() {
    int fcfs_num = 0;
    int sjf_num = 1;
    int rr_num = 2;
    schedulerTests(fcfs_num);
    schedulerTests(sjf_num);
    schedulerTests(rr_num);
    return 0;
}
