#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <alchemy/timer.h>

// Definitions:
#define NTASKS 3
#define HIGH 52  // high priority
#define MID 51   // medium priority
#define LOW 50   // low priority

#define EXECTIME 2e8 // execution time in ns
#define SPINTIME 1e7 // spin time in ns

// Global variables:
RT_TASK demo_task[NTASKS];
RT_SEM mysync;

void demo(void *arg) {
    // Initialize:
    RTIME starttime, runtime;
    int num=*(int *)arg;
    runtime = 0;
    RT_TASK_INFO task_info;

    // Print start message:
    printf("Task  : %d\n",num);

    // Wait for signal:
    rt_sem_p(&mysync,TM_INFINITE);

    // Get task information:
    rt_task_inquire(NULL,&task_info);

    // Loop to wait:
    while(runtime < EXECTIME) {
        // Check to see if this is the highest priority task and
        // if it is the middle of execution:
        if ((task_info.prio == HIGH) && (runtime/1000000 == 110)) {
            // Reassign task priorities:
            rt_task_set_priority(&demo_task[0],LOW+10);
            rt_task_set_priority(&demo_task[1],MID+10);
        }
        
        // Burn CPU cycles:
        rt_timer_spin(SPINTIME);

        // Increase runtime:
        runtime = runtime + SPINTIME;

        // Print message:
        printf("Running Task  : %d  at ms : %d\n",num,runtime/1000000);
    }

    // Print end message:
    printf("End Task  : %d\n",num);
}

//startup code
void startup() {
    // Initialize:
    int i;
    char str[10];

    // semaphore to sync task startup on
    rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);

    // Loop to create and start tasks:
    for(i=0; i < NTASKS; i++) {
        printf("start task  : %d\n",i);
        sprintf(str,"task%d",i);
        rt_task_create(&demo_task[i], str, 0, 50, 0);
        rt_task_start(&demo_task[i], &demo, &i); 
    }

    // assign priorities to tasks
    rt_task_set_priority(&demo_task[0],LOW);
    rt_task_set_priority(&demo_task[1],MID);
    rt_task_set_priority(&demo_task[2],HIGH);

    // Print wake up:
    printf("wake up all tasks\n");

    // Signal:
    rt_sem_broadcast(&mysync);
}

int main(int argc, char* argv[]) {
    // Run start up function:
    startup();

    // Wait:
    printf("\nType CTRL-C to end this program\n\n" );
    pause();
}