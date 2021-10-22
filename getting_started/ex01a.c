#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <alchemy/task.h>

RT_TASK hello_task;

// function to be executed by task
void helloWorld(void *arg)
{
  RT_TASK_INFO curtaskinfo;

  printf("Hello World!\n");

  // inquire current task
  rt_task_inquire(NULL,&curtaskinfo);

  // print task name
  printf("Task name : %s \n", curtaskinfo.name);
}

int main(int argc, char* argv[])
{
  char str[10];
  int ret_val;

  printf("start task\n");
  sprintf(str,"hello");

  /* Create task
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */
  rt_task_create(&hello_task, str, 0, 50, 0);

  /*  Start task
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  ret_val = rt_task_start(&hello_task, &helloWorld, 0);

  // Print return value of function:
  printf("(%d) %s\n",ret_val,strerror(-ret_val));
}