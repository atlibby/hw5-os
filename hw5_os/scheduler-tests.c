#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pqueue.h"

typedef struct {
    int taskID; // unique ID for this task
    int submitTime; // the time at which this task is submitted
    int totalBurstTime; // total time that this task will run
    int totalWaitTime; // the total time that this task has waited
    int remainingBurstTime; // the time that this task still needs; for RR
    int lastRunTime; // the last time this task ran; for RR
} Task;

// Implement the SJF and FCFS scheduling algorithms
int scheduler(int schedulerType, PQueueNode **taskQueue, PQueueNode **futureQueue, int quantum, int contextSwitchTime) {
    int currentTime = 0;
    int totalWaitTime = 0;

    while (*taskQueue != NULL || *futureQueue != NULL) {
        // Move tasks from futureQueue to taskQueue whose submitTime is less than currentTime
        while (*futureQueue != NULL && ((Task *)(peek(*futureQueue)))->submitTime <= currentTime) {
            Task *task = (Task *)dequeue(futureQueue);

            if (schedulerType == 0) {
                // SJF scheduling
                enqueue(taskQueue, task->totalBurstTime, task);
            } else if (schedulerType == 1) {
                // FCFS scheduling
                enqueue(taskQueue, task->submitTime, task);
            } else if (schedulerType == 2) {
                // RR scheduling
                task->lastRunTime = currentTime; // Initialize lastRunTime
                enqueue(taskQueue, 1000000, task);  // Set priority to a large value for FCFS behavior
            } else {
                printf("Invalid schedulerType\n");
                free(task);  // Free the memory allocated for the task
                return -1;   // Return an error code
            }
        }

        if (*taskQueue == NULL) {
            break;  // No tasks to process
        }

        if (schedulerType == 0 || schedulerType == 1) {
            Task *task = (Task *)dequeue(taskQueue);
            task->totalWaitTime += currentTime - task->submitTime;
            totalWaitTime += task->totalWaitTime;

            if (schedulerType == 0) {
                currentTime += task->totalBurstTime;
            } else if (schedulerType == 1) {
                currentTime += task->totalBurstTime;
            }

            free(task);  // Free the memory allocated for the task
        } else if (schedulerType == 2) {
            Task *task = (Task *)dequeue(taskQueue);

            if (task->lastRunTime > 0) {
                task->totalWaitTime += (currentTime - task->lastRunTime);
            } else {
                task->totalWaitTime = currentTime - task->submitTime;
            }

            if (quantum >= task->remainingBurstTime) {
                // Task is complete
                currentTime += task->remainingBurstTime;
                totalWaitTime += task->totalWaitTime;
                free(task);  // Free the memory allocated for the task
            } else {
                // Task needs more than the time quantum
                currentTime += quantum;
                task->remainingBurstTime -= quantum;
                task->lastRunTime = currentTime;
                enqueue(taskQueue, 1000000, task);  // Set priority to a large value for FCFS behavior

                // Add context switch time
                currentTime += contextSwitchTime;
            }
        }
    }
    return totalWaitTime;
}


void createTask(PQueueNode **futureQueue, int taskID, int submitTime, int totalBurstTime) {
    Task *task = malloc(sizeof(Task));
    task->taskID = taskID;
    task->submitTime = submitTime;
    task->totalBurstTime = totalBurstTime;
    task->totalWaitTime = 0;
    task->remainingBurstTime = totalBurstTime;
    task->lastRunTime = 0;
    enqueue(futureQueue, submitTime, task);
}

int exprand(double mean) {
    double r, t;
    int rtnval;
    r = drand48();
    t = -log(1-r) * mean;
    rtnval = (int) floor(t);
    if (rtnval == 0)
        rtnval = 1;
    return(rtnval);
}

int main(){
    PQueueNode *taskQueue = NULL;    // Task queue maintained as a priority queue
    PQueueNode *futureQueue = NULL;  // Future task queue
    static int quantum = 4;
    static int contextSwitchTime = 0;

    // Test case 1
    printf("Test case 1\n");
    createTask(&futureQueue, 1, 0, 6);
    createTask(&futureQueue, 2, 2, 8);
    createTask(&futureQueue, 3, 4, 7);
    createTask(&futureQueue, 4, 8, 3);

    int sjfWaitTime = scheduler(0, &taskQueue, &futureQueue, quantum, contextSwitchTime);
    printf("SJF Total Wait Time: %d\n", sjfWaitTime);
    printf("SJF Average Wait Time: %f\n", (float)sjfWaitTime / 4);

    // Test case 1
    createTask(&futureQueue, 1, 0, 6);
    createTask(&futureQueue, 2, 2, 8);
    createTask(&futureQueue, 3, 4, 7);
    createTask(&futureQueue, 4, 8, 3);

    int fcfsWaitTime = scheduler(1, &taskQueue, &futureQueue, quantum, contextSwitchTime);
    printf("FCFS Total Wait Time: %d\n", fcfsWaitTime);
    printf("FCFS Average Wait Time: %f\n", (float)fcfsWaitTime / 4);

    // Test case 2
    printf("\nTest case 2\n");
    createTask(&futureQueue, 1, 0, 6);
    createTask(&futureQueue, 2, 0, 8);
    createTask(&futureQueue, 3, 0, 7);
    createTask(&futureQueue, 4, 0, 3);

    sjfWaitTime = scheduler(0, &taskQueue, &futureQueue, quantum, contextSwitchTime);
    printf("SJF Total Wait Time: %d\n", sjfWaitTime);
    printf("SJF Average Wait Time: %f\n", (float)sjfWaitTime / 4);

    // Test case 2
    createTask(&futureQueue, 1, 0, 6);
    createTask(&futureQueue, 2, 0, 8);
    createTask(&futureQueue, 3, 0, 7);
    createTask(&futureQueue, 4, 0, 3);

    fcfsWaitTime = scheduler(1, &taskQueue, &futureQueue, quantum, contextSwitchTime);
    printf("FCFS Total Wait Time: %d\n", fcfsWaitTime);
    printf("FCFS Average Wait Time: %f\n", (float)fcfsWaitTime / 4);

    // Test case 2
    createTask(&futureQueue, 1, 0, 6);
    createTask(&futureQueue, 2, 0, 8);
    createTask(&futureQueue, 3, 0, 7);
    createTask(&futureQueue, 4, 0, 3);

    int rrWaitTime = scheduler(2,&taskQueue, &futureQueue, quantum, contextSwitchTime);
    printf("RR Total Wait Time: %d\n", rrWaitTime);
    printf("RR Average Wait Time: %f\n", (float)rrWaitTime / 4);

    // Graph the relationship between context-switch time and throughput or mean wait time.
    printf("\nGraph the relationship between context-switch time and throughput or mean wait time:\n");
    printf("Context Switch Time, RR Mean Wait Time\n");
    for (int i = 0; i <= 50; i++) {
        createTask(&futureQueue, 1, 0, 6);
        createTask(&futureQueue, 2, 0, 8);
        createTask(&futureQueue, 3, 0, 7);
        createTask(&futureQueue, 4, 0, 3);

        contextSwitchTime = i;
        rrWaitTime = scheduler(2,&taskQueue, &futureQueue, quantum, contextSwitchTime);
        printf("%d,%f\n", contextSwitchTime, (float)rrWaitTime / 4);
    }


    return 0;
}
