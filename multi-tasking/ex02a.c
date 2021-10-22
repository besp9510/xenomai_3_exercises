///////////////////////////////////////////////////////////////////////////////
//
// Multi-tasking Example 2a
//
// Running five tasks in one-shot mode
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
RT_TASK task_4;
RT_TASK task_5;

// Task function
void task(void *arg) {
	// Initialize:
    RT_TASK_INFO curtaskinfo;

    // Get task information:
    rt_task_inquire(NULL,&curtaskinfo);

    // Print task name:
    rt_printf("Task name: %s \n", curtaskinfo.name);

    return;
}

void main(int argc, char const *argv[])
{
	// Create tasks:
	rt_task_create(&task_1,"task_1",0,1,0);
	rt_task_create(&task_2,"task_2",0,1,0);
	rt_task_create(&task_3,"task_3",0,1,0);
	rt_task_create(&task_4,"task_4",0,1,0);
	rt_task_create(&task_5,"task_5",0,1,0);

	// Start tasks:
	rt_task_start(&task_1,&task,0);
	rt_task_start(&task_2,&task,0);
	rt_task_start(&task_3,&task,0);
	rt_task_start(&task_4,&task,0);
	rt_task_start(&task_5,&task,0);				

	return;
}