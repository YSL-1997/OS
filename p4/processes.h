#ifndef PROCESSES_H_
#define PROCESSES_H_

typedef struct process{
  unsigned long pid;
  // stores the pid of the process

  unsigned long start_index;
  unsigned long end_index;
}process;


#endif