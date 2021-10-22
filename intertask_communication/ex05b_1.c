///////////////////////////////////////////////////////////////////////////////
//
// Intertask Communication Example
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

void taskOne(void* arg) {
    // Initialize:
    int retval;
    
    char message[] = "Message from taskOne";

    //task message blocks
    RT_TASK_MCB mymcb, talk_reply;

    mymcb.data = message;
    mymcb.size = sizeof(message);

    talk_reply.size = 0;
    talk_reply.data = NULL;

    retval = rt_task_send(&task_struct[1],&mymcb,&talk_reply,TM_INFINITE);
    
    if (retval == -EWOULDBLOCK ) {
       rt_printf("Would block error: %d\n", retval);
    } else if(retval == -ETIMEDOUT){
        rt_printf("Timedout error: %d\n", retval);
    } else if(retval == -ENOBUFS){
        rt_printf("unblocked called error: %d\n", retval);
    } else if(retval == -EIDRM){
        rt_printf("sleep error: %d\n", retval);
    } else if(retval == -ESRCH){
        rt_printf("Buffer error: %d\n", retval);
    } else {
       rt_printf("taskOne sent message\n");
    }

    return;
}

void taskTwo(void* arg) {
    int retval;
    char msgBuf[MAX_MESSAGE_LENGTH];
    char message[] = "Message from taskTwo";
    RT_TASK_MCB mymcb, listen_reply;
  
    mymcb.data = msgBuf;
    mymcb.size = sizeof(msgBuf);
    
    /* receive message */
    retval = rt_task_receive(&mymcb,TM_INFINITE);   

    //error checks
    if (retval == -EWOULDBLOCK ) {
       rt_printf("Would block error: %d\n", retval);
    } else if(retval == -ETIMEDOUT){
        rt_printf("Timedout error: %d\n", retval);
    } else if(retval == -ENOBUFS){
        rt_printf("unblocked called error: %d\n", retval);
    } else if(retval == -EIDRM){
        rt_printf("sleep error: %d\n", retval);
    } else if(retval == -ESRCH){
        rt_printf("Buffer error: %d\n", retval);
    } else if (retval < 0 ) {
        rt_printf("Receiving error\n");
    } else {
        rt_printf("taskTwo received message: %s\n",mymcb.data);
        rt_printf("with length %d\n",mymcb.size);
    }

    listen_reply.size = 0;
    listen_reply.data = NULL;
    rt_task_reply(retval, &listen_reply);

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

    // Loop to create tasks:
    for(i=0; i < NTASKS; i++) {
        sprintf(str,"task%d",i);
        rt_task_create(&task_struct[i],str,0,50,0);
    }

    // Loop to start tasks:
    for(i=0; i < NTASKS; i++) {
        rt_printf("start task  : %d\n",i+1);
        rt_task_start(&task_struct[i],task_func[i],&i);
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