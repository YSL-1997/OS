/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

int main(int argc, char *argv[])
{
  void *proc_table;

  // unsigned long frame_size = option_parser(argc, argv);
  // printf("frame_size %lu\n", frame_size);
  unsigned long frame_size = 1024;
  process *runnable_head = malloc(sizeof(struct process));
  process *runnable_tail = malloc(sizeof(struct process));
  runnable_head = NULL;
  runnable_tail = NULL;
  proc_table = get_processes_info(&runnable_head, &runnable_tail);

  fifo(&runnable_head, &runnable_tail, frame_size, &proc_table);


  return 0;
}