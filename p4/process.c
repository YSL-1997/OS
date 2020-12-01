/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

/*
  This module is for process
*/

#include "process.h"
#include "errorHandling.h"

// create a process pointer and initialize with pid 
process* create_process(char* pid)
{
  process* ptr = (process*)malloc(sizeof(process));
  handle_malloc_error(ptr);
  
  ptr->pid = pid;
  ptr->start_index = 0;
  ptr->end_index = 0;
  ptr->cur_index = 0;
  ptr->io_next = NULL;
  ptr->runnable_next = NULL;
  ptr->runnable_prev = NULL;
  ptr->is_blocked = false;
  ptr->timer = 0;
  ptr->blocked_vpn = "";

  return ptr;
}