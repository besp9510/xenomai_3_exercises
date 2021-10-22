///////////////////////////////////////////////////////////////////////////////
//
// Multi-tasking Example 2d
//
// Running three tasks in periodic mode
//
// -------------------------------------------------------------------------- /
//
// Dependencies:
// - alchemy/task header
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
#include <errno.h>

#include <alchemy/task.h>

// Define tasks:
RT_TASK task_1;
RT_TASK task_2;
RT_TASK task_3;

// Task function
void task(void *arg) {
    // Delay task execution:
    rt_task_sleep(1000000000); // 1 second

	// Initialize:
    RT_TASK_INFO curtaskinfo;

    // Get task information:
    rt_task_inquire(NULL,&curtaskinfo);

    // Infinite loop
    while (1) {
		// Print task name:
	    rt_printf("Task name: %s \n", curtaskinfo.name);
	    
	    // Release processor and wait for next period to execute again:
	    rt_task_wait_period(NULL);
  	}

    return;
}

void main(int argc, char const *argv[])
{
	// Create tasks:
	rt_task_create(&task_1,"task_1",0,1,0);
	rt_task_create(&task_2,"task_2",0,1,0);
	rt_task_create(&task_3,"task_3",0,1,0);

	// Set tasks as periodic:
	rt_task_set_periodic(&task_1, TM_NOW, 1000000000); // 1 seconds period
	rt_task_set_periodic(&task_2, TM_NOW, 2000000000); // 2 seconds period
	rt_task_set_periodic(&task_3, TM_NOW, 3000000000); // 3 seconds period

	// Start tasks:
	rt_task_start(&task_1,&task,0);
	rt_task_start(&task_2,&task,0);
	rt_task_start(&task_3,&task,0);			

	// Wait:
    printf("end program by CTRL-C\n");
    pause();

	return;
}