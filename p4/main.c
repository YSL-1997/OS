/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "basic.h"

int main(int argc, char *argv[])
{
  void *proc_table = NULL;
  unsigned long num_process = 0;

  unsigned long frame_size = option_parser(argc, argv);
  printf("frame_size %lu\n", frame_size);

  process *runnable_head = malloc(sizeof(struct process));
  process *runnable_tail = malloc(sizeof(struct process));
  runnable_head = NULL;
  runnable_tail = NULL;
  proc_table = get_processes_info(&runnable_head, &runnable_tail, &num_process,
                                  argv[argc - 1]);
  printf("process table created\n");

  statistics *stat = stat_init(num_process);
  printf("num_process %ld\n", num_process);
  basic(&runnable_head, &runnable_tail, frame_size, &proc_table, stat,
        argv[argc - 1]);

  printf("AMU = %f\n", stat->fake_AMU / (frame_size * stat->RTime));
  printf("ARP = %f\n", stat->fake_ARP / stat->RTime);

  printf("exit success!\n");

  return 0;
}
