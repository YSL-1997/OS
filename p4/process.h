#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>

typedef struct process{
  char* pid;
  // stores the pid of the process

  char* blocked_vpn;
  // stores the vpn corresponds to this pid that gets blocked
  
  unsigned long start_index;
  // stores the starting index of the pid
  
  unsigned long end_index;
  // stores the end index of the pid

  unsigned long cur_index;
  // stores the current index of the pid that about to execute
  // e.g. on line_index 0, pid 1 has a page fault, then pid_1->cur_index = 0
  // then add to the I/O queue

  struct process* io_next;
  // this is used in the I/O queue

  struct process* runnable_next;
  // this is for the runnable list(double linked list)

  struct process* runnable_prev;
  // this is for the runnable list(double linked list)
  
  bool is_blocked;
  // indicates whether the process is blocked or not

  unsigned long timer;
  // stores the time before I/O completes
  
}process;


#endif
