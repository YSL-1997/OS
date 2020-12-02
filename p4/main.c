/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

int main(int argc, char *argv[])
{
  void *proc_table=NULL;

  unsigned long frame_size = option_parser(argc, argv);
  printf("frame_size %lu\n", frame_size);

  process *runnable_head = malloc(sizeof(struct process));
  process *runnable_tail = malloc(sizeof(struct process));
  runnable_head = NULL;
  runnable_tail = NULL;
  proc_table = get_processes_info(&runnable_head, &runnable_tail);
  printf("process table created\n");
  fifo(&runnable_head, &runnable_tail, 2, &proc_table);

  printf("exit success!\n");

  return 0;
}


// int main(){
//   void* pt;
//   page* p1 = initialize_page_frame(1);
//   p1->pid = "1";
//   p1->vpn = "123";
//   node_pt* entry1 = create_entry_pt(p1);
//   add_to_pt(&pt, entry1);
//   printf("add to pt success\n");
//   find_pt(&pt, p1->pid);
//   printf("find success\n");
//   return 0;
// }