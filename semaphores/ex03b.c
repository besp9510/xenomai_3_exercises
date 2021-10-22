#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

// Definitions
RT_TASK t1; // Task
RT_TASK t2; // Task
RT_SEM sem; // Semaphore

// Global variables:
int global = 0;
int ITER = 10;

void taskOne(void *arg) {
    // Initialize:
    int i;

    for (i=0; i < ITER; i++) {
        // Wait for signal:
        rt_sem_p(&sem,TM_INFINITE);

        // Increase and print global:
        printf("Task One: Global = %d \n", ++global);

        // Signal semaphore:
        rt_sem_v(&sem);
    }
}

void taskTwo(void *arg) {
    // Initialize:
    int i;

    for (i=0; i < ITER; i++) {
        // Wait for signal:
        rt_sem_p(&sem,TM_INFINITE);

        // Increase and print global:
        printf("Task Two: Global = %d \n", --global);

        // Signal semaphore:
        rt_sem_v(&sem);
    }
}

int main(int argc, char* argv[]) {
    // Create tasks:
    rt_task_create(&t1, "task1", 0, 1, 0);
    rt_task_create(&t2, "task2", 0, 2, 0);

    // Create semaphore:
    rt_sem_create(&sem, "semaphore", 0, S_FIFO);

    // Start Tasks:
    rt_task_start(&t1, &taskOne, 0);
    rt_task_start(&t2, &taskTwo, 0);

    // Signal tasks to start:
    rt_sem_v(&sem);

    // Wait:
    sleep(0.5);

    return 0;
}