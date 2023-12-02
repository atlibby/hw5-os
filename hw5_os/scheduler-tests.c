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

// Implement the SJF and FCFS scheduling algorithms
int scheduler(int schedulerType, PQueueNode **taskQueue, PQueueNode **futureQueue, int quantum, int contextSwitchTime) {
    int currentTime = 0;
    int totalWaitTime = 0;
    int totalQueueLength = 0;
    int enqueueEvents = 0;

    while (*taskQueue != NULL || *futureQueue != NULL) {
        // Move tasks from futureQueue to taskQueue whose submitTime is less than currentTime
        while (*futureQueue != NULL && ((Task *)(peek(*futureQueue)))->submitTime <= currentTime) {
            Task *task = (Task *)dequeue(futureQueue);

            totalQueueLength += task->queueLength;
            enqueueEvents++;

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

    /*if (enqueueEvents > 0) {
        float meanQueueLength = (float)totalQueueLength / enqueueEvents;
        printf("Mean Queue Length: %f\n", meanQueueLength);
    } else {
        printf("No enqueue events occurred.\n");
    }*/

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

void createRandomTasks(PQueueNode **futureQueue, Task **tasks, int n, double meanBurstTime, double meanInterarrivalTime) {
    // Seed the random number generator with the current time
    srand48(time(NULL));

    int currentTime = 0;

    for (int i = 0; i < n; ++i) {
        Task *task = malloc(sizeof(Task));
        task->taskID = i + 1;
        task->submitTime = currentTime;
        task->totalBurstTime = exprand(meanBurstTime);
        task->totalWaitTime = 0;
        task->remainingBurstTime = task->totalBurstTime;
        task->lastRunTime = 0;
        task->queueLength = 0;

        tasks[i] = task;

        // Enqueue the task into the futureQueue
        enqueue(futureQueue, currentTime, &tasks[i]);

        // Update the current time for the next task
        currentTime += exprand(meanInterarrivalTime);
    }
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


    // Create n random tasks
    int n = 50; // Number of tasks
    double meanBurstTime = 10.0; // Adjust as needed
    double meanInterarrivalTime = 5.0; // Adjust as needed

    Task *tasks[n];

    createRandomTasks(&futureQueue, &tasks, n, meanBurstTime, meanInterarrivalTime);

    // Print the generated tasks
    printf("Generated Tasks:\n");
    printf("TaskID\tSubmitTime\tTotalBurstTime\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t%d\t\t%d\n", tasks[i]->taskID, tasks[i]->submitTime, tasks[i]->totalBurstTime);
        rrWaitTime = scheduler(2, &taskQueue, &futureQueue, quantum, contextSwitchTime);

    }
    return 0;
}
