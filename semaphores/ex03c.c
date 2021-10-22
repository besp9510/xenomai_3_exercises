///////////////////////////////////////////////////////////////////////////////
//
// Multi-tasking Example 3c
//
// Semaphore Task Priority
//
// -------------------------------------------------------------------------- /
//
// Dependencies:
// - alchemy/task header
// - alchemy/sem header
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

#include <alchemy/task.h>
#include <alchemy/sem.h>

// Define tasks:
RT_TASK task_1;
RT_TASK task_2;
RT_TASK task_3;
RT_TASK task_4;
RT_TASK task_5;

RT_SEM sem[5];

// Global:
int arg1 = 10;
int arg2 = 20;
int arg3 = 7;
int arg4 = 65;
int arg5 = 2;

// Task function
void task(void *arg) {
	// Get argument:
	int num =  * (int *)arg;
	int index = 1;

	// Initialize:
    RT_TASK_INFO curtaskinfo;

    // Get task information:
    rt_task_inquire(NULL,&curtaskinfo);

    // Get correct index:
    if (strcmp(curtaskinfo.name,"task_1") == 0) {
    	index = 0;
    } 
    else if (strcmp(curtaskinfo.name,"task_2") == 0) {
    	index = 1;
    }
    else if (strcmp(curtaskinfo.name,"task_3") == 0) {
    	index = 2;
    }
    else if (strcmp(curtaskinfo.name,"task_4") == 0) {
    	index = 3;
    }
    else if (strcmp(curtaskinfo.name,"task_5") == 0) {
    	index = 4;
    }

    // Wait for signal to start:
    rt_sem_p(&sem[index],TM_INFINITE);

    // Print:
    rt_printf("%s: argument = %d \n",curtaskinfo.name,num);

    if (strcmp(curtaskinfo.name,"task_1") != 0) {
    	// Start the next task
		rt_sem_v(&sem[index-1]);
	}

    return;
}

void main(int argc, char const *argv[])
{
	// Initialize:
	int i;
	char str[10];

	// Loop to create semaphores:
	for (i = 0; i < 5; ++i) {
		sprintf(str,"semaphore%d",i);
		rt_sem_create(&sem[i],str,0,S_FIFO);
	}

	// Create tasks:
	rt_task_create(&task_1,"task_1",0,50,0); // Lowest priority
	rt_task_create(&task_2,"task_2",0,50,0);
	rt_task_create(&task_3,"task_3",0,50,0);
	rt_task_create(&task_4,"task_4",0,50,0);
	rt_task_create(&task_5,"task_5",0,50,0); // Highest priority

	// Start tasks:
	rt_task_start(&task_1,&task,&arg1);
	rt_task_start(&task_2,&task,&arg2);
	rt_task_start(&task_3,&task,&arg3);
	rt_task_start(&task_4,&task,&arg4);
	rt_task_start(&task_5,&task,&arg5);	

	// Signal semaphore 5:
	rt_sem_v(&sem[4]);

	sleep(1);			

	return;
}