#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <alchemy/timer.h>

// Definitions:
#define NTASKS 4
#define EXECTIME 2e8   // execution time in ns
#define SPINTIME 1e7   // spin time in ns

// Global:
RT_TASK demo_task[NTASKS];
RT_SEM mysync;

void demo(void *arg) {
    // Initialize:
    RTIME starttime, runtime;
    int num= * (int *) arg;

    // Set time credit alloted for current task:
    rt_task_slice(NULL,1e5); // Nanoseconds

    // Check to see if fourth task:
    if (num == 3) {
        // Reassign priority:
        rt_task_set_priority(NULL,80);
    }

    // Start message:
    printf("Task  : %d\n",num);

    // Wait for signal:
    rt_sem_p(&mysync,TM_INFINITE);

    // let the task run RUNTIME ns in steps of SPINTIME ns
    runtime = 0;

    // Loop to runtime:
    while(runtime < EXECTIME) {
        // Spin CPU
        rt_timer_spin(SPINTIME); 

        // Increment runtime:
        runtime = runtime + SPINTIME;

        // Print runtime:
        printf("Running Task  : %d  at time ms : %d\n",num,runtime/1000000);
    }

    // End message:
    printf("End Task  : %d\n",num);
}

//startup code
void startup() {
    // Initialize:
    int i;
    char str[10] ;
 
    // Enable round robin scheduling:
    rt_task_set_mode(0,XNRRB,NULL);

    // semaphore to sync task startup on
    rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);

    // Loop to create and start tasks:
    for(i=0; i < NTASKS; i++) {
        printf("start task  : %d\n",i);
        sprintf(str,"task%d",i);
        rt_task_create(&demo_task[i], str, 0, 50, 0);
        rt_task_start(&demo_task[i], &demo, &i);
    }

    // Print message:
    printf("wake up all tasks\n");

    // Signal:
    rt_sem_broadcast(&mysync);
}   

int main(int argc, char* argv[]) {
    // Call startup function:
    startup();

    // Wait:
    printf("\nType CTRL-C to end this program\n\n" );
    pause();
}