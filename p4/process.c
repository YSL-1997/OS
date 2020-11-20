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

process* create_process(unsigned long pid)
{
  process* ptr = (process*)malloc(sizeof(process));
  handle_malloc_error(ptr);
  
  ptr->pid = pid;
  ptr->start_index = 0;
  ptr->end_index = 0;
  ptr->cur_index = 0;
  ptr->io_next = NULL;
  ptr->is_blocked = false;

  return ptr;
}