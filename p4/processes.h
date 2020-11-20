#ifndef PROCESSES_H_
#define PROCESSES_H_

typedef struct process{
  unsigned long pid;
  // stores the pid of the process

  unsigned long start_index;
  // stores the starting index of the pid
  
  unsigned long end_index;
  // stores the end index of the pid

  unsigned long cur_index
  // stores the current index of the pid that about to execute
  // e.g. on line_index 0, pid 1 has a page fault, then pid_1->cur_index = 0
  // then add to the I/O queue

  process* next;
  // this is used in the I/O queue
  
}process;


#endif