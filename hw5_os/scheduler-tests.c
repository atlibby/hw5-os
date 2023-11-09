#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


int checkResult(Task *testTask, Task *expectedTask) {
    int rtnval = 0;

    if (testTask == NULL) {
        printf("ERROR: checking result; have NULL pointer\n");
        rtnval = 1;
    } else {
        if (testTask->taskID != expectedTask->taskID) {
            printf("ERROR: checking result; expected '%d' but got '%d'\n",
                   expectedTask->taskID, testTask->taskID);
            rtnval = 1;
        }
        if (testTask->submitTime != expectedTask->submitTime) {
            printf("ERROR: checking result; expected %d but got %d\n",
                   expectedTask->submitTime, testTask->submitTime);
            rtnval = 1;
        }
        if (testTask->totalBurstTime != expectedTask->totalBurstTime) {
            printf("ERROR: checking result; expected %d but got %d\n",
                   expectedTask->totalBurstTime, testTask->totalBurstTime);
            rtnval = 1;
        }
        if (testTask->totalWaitTime != expectedTask->totalWaitTime) {
            printf("ERROR: checking result; expected %d but got %d\n",
                   expectedTask->totalWaitTime, testTask->totalWaitTime);
            rtnval = 1;
        }
    }
    return rtnval;
}

int schedulerTests() {
    int minPriority;
    SchedulerNode *scheduler = NULL;
    Task *task;
    int rc, numfails;

    numfails = 0;

    Task *task1 = createTask(1, 0, 5);
    enqueue(&scheduler, 3, task1);

    Task *task2 = createTask(2, 1, 3);
    enqueue(&scheduler, 8, task2);

    Task *task3 = createTask(3, 2, 6);
    enqueue(&scheduler, 1, task3);

    Task *task4 = createTask(4, 3, 4);
    enqueue(&scheduler, 3, task4);

    Task *task5 = createTask(5, 4, 2);
    enqueue(&scheduler, 5, task5);

    Task *task6 = createTask(6, 5, 1);
    enqueue(&scheduler, 7, task6);

    Task *task7 = createTask(7, 6, 3);
    enqueue(&scheduler, 2, task7);

    Task *task8 = createTask(8, 7, 2);
    enqueue(&scheduler, 4, task8);

    Task *task9 = createTask(9, 8, 4);
    enqueue(&scheduler, 6, task9);

    task = peek(scheduler);
    printf("peek: ");
    printTask(scheduler);
    rc = checkResult(task, task3);
    if (rc != 0)
        ++numfails;

    task = dequeue(&scheduler);
    printf("dequeued: ");
    printTask(scheduler);
    rc = checkResult(task, task3);
    if (rc != 0)
        ++numfails;

    task = peek(scheduler);
    printf("peek: ");
    printTask(task);
    rc = checkResult(task, task1);
    if (rc != 0)
        ++numfails;

    task = dequeue(&scheduler);
    printf("dequeued: ");
    printTask(task);
    rc = checkResult(task, task1);
    if (rc != 0)
        ++numfails;

    task = peek(scheduler);
    printf("peek: ");
    printTask(task);
    rc = checkResult(task, task4);
    if (rc != 0)
        ++numfails;

    printScheduler(scheduler, printTask);



    // Test peek
    task = peek(scheduler);
    if (task == NULL) {
        printf("ERROR: peek returned NULL\n");
        numfails++;
    } else {
        if (task->taskID != 3) {
            printf("ERROR: peek returned task with ID %d, expected 3\n", task->taskID);
            numfails++;
        }
    }

// Test getMinPriority
    minPriority = getMinPriority(scheduler);
    if (minPriority != 1) {
        printf("ERROR: getMinPriority returned %d, expected 1\n", minPriority);
        numfails++;
    }
// First come first serve (FCFS) and shortest job first (SJF) scheduling algorithm


int main() {
    printf("Hello, World!\n");
    return 0;
}
