/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "basic.h"

int main(int argc, char *argv[])
{
  void *proc_table = NULL; // process table
  unsigned long num_process = 0; // # of processes
  unsigned long frame_size = option_parser(argc, argv);

  process *runnable_head = NULL;
  process *runnable_tail = NULL;

  // create process table
  proc_table = get_processes_info(&runnable_head, &runnable_tail, &num_process,
                                  argv[argc - 1]);

  // initialize stats
  statistics *stat = stat_init(num_process);

  // start execution
  basic(&runnable_head, &runnable_tail, frame_size, &proc_table, stat,
        argv[argc - 1]);
  
  // print out the stats
  printf("AMU = %f\n", stat->fake_AMU / (frame_size * stat->RTime));
  printf("ARP = %f\n", stat->fake_ARP / stat->RTime);
  printf("TMR = %ld\n", stat->TMR);
  printf("TPI = %ld\n", stat->TPI);
  printf("RTime = %ld\n", stat->RTime);

  return 0;
}
