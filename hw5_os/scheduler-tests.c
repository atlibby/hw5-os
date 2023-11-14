#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scheduler.atlibby.alynkirscht.h"

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

int schedulerTests() {
    int minPriority;
    SchedulerNode *taskQueue = NULL;
    SchedulerNode *futureQueue = NULL;
    Task *task;

//    Task *task1 = createTask(1, 0, 6);
//    enqueue(&taskQueue, 3, task1);
//
//    Task *task2 = createTask(2, 2, 8);
//    enqueue(&taskQueue, 8, task2);
//
//    Task *task3 = createTask(3, 4, 7);
//    enqueue(&taskQueue, 1, task3);
//
//    Task *task4 = createTask(4, 8, 3);
//    enqueue(&taskQueue, 3, task4);

    int currentTime = 0;

    int totalWaitTime = 0;

    int currentID = 0;

    bool done;

// First come, first served (FCFS) scheduling algorithm
    printf("First come first served (FCFS) scheduling algorithm\n");
    taskQueue = NULL;
    futureQueue = NULL;

    Task *task1 = createTask(1, 0, 6);
    enqueue(&taskQueue, 0, task1);

    Task *task2 = createTask(2, 2, 8);
    enqueue(&taskQueue, 2, task2);

    Task *task3 = createTask(3, 4, 7);
    enqueue(&taskQueue, 4, task3);

    Task *task4 = createTask(4, 8, 3);
    enqueue(&taskQueue, 8, task4);

    currentTime = 0;

    totalWaitTime = 0;

    currentID = 1;

    task1->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task1->submitTime, task1);

    task2->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task2->submitTime, task2);

    task3->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task3->submitTime, task3);

    task4->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task4->submitTime, task4);

    done = false;

    while (!done) {
        if (taskQueue == NULL) {
            printf("Scheduler is empty\n");
            done = true;
        }

        else {
            minPriority = getMinPriority(taskQueue);
            while (currentTime < minPriority) {
                printf("Time %d: CPU idle\n", currentTime);
                currentTime++;
                if (futureQueue != NULL) {
                    while (getMinPriority(futureQueue) < currentTime) {
                        task = (Task *) dequeue(&futureQueue);
                        currentTime = task->submitTime;
                        printf("Time %d: Task %d arrived\n", currentTime, task->taskID);

                    }
                }
            }
            task = (Task *) dequeue(&taskQueue);
            printf("Time %d: Task %d is running\n", currentTime, task->taskID);
            task->totalWaitTime += currentTime - task->submitTime;
            currentTime += task->totalBurstTime;
            printf("Time %d: Task %d finished\n", currentTime, task->taskID);
            totalWaitTime += task->totalWaitTime;
            free(task);
        }
    }
    printf("FCFS total wait time: %d\n", totalWaitTime);

    // Shortest Job First (SJF) scheduling algorithm
    printf("Shortest Job First (SJF) scheduling algorithm\n");
    taskQueue = NULL;
    futureQueue = NULL;

    task1 = createTask(1, 0, 6);
    enqueue(&taskQueue, 6, task1);

    task2 = createTask(2, 2, 8);
    enqueue(&taskQueue, 8, task2);

    task3 = createTask(3, 4, 7);
    enqueue(&taskQueue, 7, task3);

    task4 = createTask(4, 8, 3);
    enqueue(&taskQueue, 3, task4);

    currentTime = 0;

    totalWaitTime = 0;

    currentID = 1;

    task1->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task1->submitTime, task1);

    task2->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task2->submitTime, task2);

    task3->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task3->submitTime, task3);

    task4->taskID = currentID;

    currentID += 1;

    enqueue(&futureQueue, task4->submitTime, task4);

    done = false;

    while (!done) {
        if (taskQueue == NULL) {
            printf("Scheduler is empty\n");
            done = true;
        }

        else {
            minPriority = getMinPriority(taskQueue);
            while (currentTime < minPriority) {
                printf("Time %d: CPU idle\n", currentTime);
                currentTime++;
            }
            if (futureQueue != NULL) {
                while (getMinPriority(futureQueue) < currentTime) {
                    task = (Task *) dequeue(&futureQueue);
                    currentTime = task->submitTime;
                    printf("Time %d: Task %d arrived\n", currentTime, task->taskID);
                }
            }
        }
        task = (Task *) dequeue(&taskQueue);
        printf("Time %d: Task %d is running\n", currentTime, task->taskID);
        task->totalWaitTime += currentTime - task->submitTime;
        totalWaitTime += task->totalWaitTime;
        currentTime += task->totalBurstTime;
        printf("Time %d: Task %d finished\n", currentTime, task->taskID);
        free(task);
    }
    printf("SJF total wait time: %d\n", totalWaitTime);
    return 0;
}

int main() {
    schedulerTests();
    return 0;
}
