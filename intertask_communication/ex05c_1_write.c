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
#include <fcntl.h>
#include <errno.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <alchemy/pipe.h>

#include <trank/rtdk.h>

// Definitions:
#define NTASKS 2
#define QUEUE_SIZE 255 // bytes
#define MAX_MESSAGE_LENGTH 40

// Tasks:
RT_TASK task;

RT_PIPE mypipe;

void write_task(void *arg) {
    // Initialize:
    int ret_val;

    // Define message:
    char message[] = "Message from real-time task";

    // Create real-time pipe:
    ret_val = rt_pipe_create(&mypipe,"rtp0",P_MINOR_AUTO,0);

    // Write message to pipe:
    ret_val = rt_pipe_write(&mypipe,&message,sizeof(message),P_NORMAL);

    // Check success:
    if (ret_val < 0) {
        printf("Error\n");
    } else if (ret_val == sizeof(message)) {
        printf("Task sent message\n");
    }

}

// Startup
void startup() {
    // Create task:
    rt_task_create(&task,"task",0,50,0);

    // Start task:
    rt_printf("Task started\n");
    rt_task_start(&task,&write_task,0);

    return;
}

int main(int argc, char* argv[]) {

    //startup code
    startup();

    // Pause:
    rt_printf("\nType CTRL-C to end this program\n\n");
    pause();

    return 0;
}
