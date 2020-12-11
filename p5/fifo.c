/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

/* 
  this function is the lru replacement algorithm
  move the page from head to tail
  input: address of the ram_head, ram_tail, clock_hand and flag
  return: the pointer to the page to be replaced

*/
page *page_replace(page **ram_head, page **ram_tail,
                   __attribute__((unused)) page **clock_hand,
                   __attribute__((unused)) int *flag,
                   __attribute__((unused)) void* pt, 
                   __attribute__((unused)) char *filename, 
                   __attribute__((unused)) void **proc_table,
                    __attribute__((unused)) unsigned long frame_size)
{
  page *ret = *ram_head;

  // move the head to tail
  move_to_ram_tail(ram_head, ram_tail);
  return ret;
}

/* 
  fifo page reference
  move the referenced page to the ram_tail
  input: pointer to a page, ram head, ram tail
*/
void page_reference(page *ptr,
                    __attribute__((unused)) page **ram_head,
                    __attribute__((unused)) page **ram_tail,
                    void *proc_table, FILE *fp)
{
  // update the cur_index of the corresponding process
  node_proc *result_proc = find_proc(proc_table, ptr->pid);
  if (result_proc == NULL)
  {
    // not found in process table => process has terminated, skip this line
  }
  else{
    result_proc->value->cur_index = handle_ftell_error(ftell(fp));
  }
  return;
}