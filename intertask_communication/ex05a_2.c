///////////////////////////////////////////////////////////////////////////////
//
// Intertask Communication
//
// -------------------------------------------------------------------------- /
//
// Dependencies:
// - alchemy/task header
// - alchemy/sem header
// - alchemy/timer header
// - alchemy/mutex header
// - alchemy/queue header
// - trank/rtdk header
//
// Input Arguments:
// - N/A
//
// Output Arguments:
// - N/A
// 
// -------------------------------------------------------------------------- /
//
// Benjamin Spencer
// ASEN 4018
// Project HEPCATS
// Subsystem: C&DH
// Created: December 20, 2018
// 
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <alchemy/mutex.h>
#include <alchemy/queue.h>

#include <trank/rtdk.h>

// Definitions:
#define NTASKS 2
#define QUEUE_SIZE 255 // bytes
#define MAX_MESSAGE_LENGTH 40

// Tasks:
RT_TASK task_struct[NTASKS];

// Queues:
RT_QUEUE myqueue;
RT_QUEUE myqueue1;

void taskOne(void *arg) {
    // Initialize:
    int ret_val;
    char msgBuf[MAX_MESSAGE_LENGTH];
    char message[] = "Message from taskOne";

    // Send message:
    ret_val = rt_queue_write(&myqueue,message,sizeof(message),Q_NORMAL);

    // Check success:
    if (ret_val < 0 ) {
        // Error:
        rt_printf("Sending error\n");
    } else {
        // Success:
        rt_printf("taskOne sent message to mailbox\n");
    }

    // Receive message:
    ret_val = rt_queue_read(&myqueue1,msgBuf,sizeof(msgBuf),TM_INFINITE);

    // Check success:
    if (ret_val < 0 ) {
        // Error:
        rt_printf("Receiving error\n");
    } else {
        // Success:
        rt_printf("taskOne received message: %s\n",msgBuf);
        rt_printf("with length %d\n",ret_val);
    }
}

void taskTwo(void *arg) {
    // Initialize:
    int ret_val;
    char msgBuf[MAX_MESSAGE_LENGTH];
    char message[] = "Message from taskTwo";

    // Send message:
    ret_val = rt_queue_write(&myqueue1,message,sizeof(message),Q_NORMAL);

    // Check success:
    if (ret_val < 0 ) {
        // Error:
        rt_printf("Sending error\n");
    } else {
        // Success:
        rt_printf("taskTwo sent message to mailbox\n");
    }

    // Receive message:
    ret_val = rt_queue_read(&myqueue,msgBuf,sizeof(msgBuf),TM_INFINITE);

    // Check success:
    if (ret_val < 0 ) {
        // Error:
        rt_printf("Receiving error\n");
    } else {
        // Success:
        rt_printf("taskTwo received message: %s\n",msgBuf);
        rt_printf("with length %d\n",ret_val);
    }

    return;
}

// Startup
void startup() {
    // Initialize:
    int i;
    char str[10];
    void (*task_func[NTASKS]) (void *arg);

    // Define task functions:
    task_func[0]=taskOne;
    task_func[1]=taskTwo;

    // Create a message queue:
    rt_queue_create(&myqueue,"myqueue",QUEUE_SIZE,10,Q_FIFO);

    // Create a message queue:
    rt_queue_create(&myqueue1,"myqueue1",QUEUE_SIZE,10,Q_FIFO);

    // Loop to create and start tasks:
    for(i=0; i < NTASKS; i++) {
        rt_printf("start task  : %d\n",i+1);
        sprintf(str,"task%d",i);
        rt_task_create(&task_struct[i], str, 0, 50, 0);
        rt_task_start(&task_struct[i], task_func[i], &i);
    }

    return;
}

int main(int argc, char* argv[]) {
    //startup code
    startup();

    // Pause:
    rt_printf("\nType CTRL-C to end this program\n\n" );
    pause();

    return 0;
}