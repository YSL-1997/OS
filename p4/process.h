#ifndef PROCESS_H_
#define PROCESS_H_

typedef struct process{
  unsigned long pid;
  // stores the pid of the process

  unsigned long start_index;
  // stores the starting index of the pid
  
  unsigned long end_index;
  // stores the end index of the pid

  unsigned long cur_index;
  // stores the current index of the pid that about to execute
  // e.g. on line_index 0, pid 1 has a page fault, then pid_1->cur_index = 0
  // then add to the I/O queue

  process* io_next;
  // this is used in the I/O queue

  process* runnable_next;
  // this is for the runnable list
  
  bool is_blocked;
  // indicates whether the process is blocked or not

}process;


#endif
