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
                   __attribute__((unused)) void *pt,
                   __attribute__((unused)) char *filename,
                   __attribute__((unused)) void **proc_table,
                   __attribute__((unused)) unsigned long frame_size)
{
  page *ret = *ram_head;
  // printf("replace page: (%s %s, %ld)\n", ret->pid,ret->vpn, ret->ppn);

  // move the head to tail
  move_to_ram_tail(ram_head, ram_tail);
  return ret;
}

/* 
  lru page reference
  move the referenced page to the ram_tail
*/
void page_reference(page *ptr, page **ram_head, page **ram_tail,
                    void *proc_table, FILE *fp)
{
  assert(*ram_head != NULL);
  assert(*ram_tail != NULL);

  // update the cur_index of the corresponding process
  node_proc *result_proc = find_proc(proc_table, ptr->pid);
  if (result_proc == NULL)
  {
    // not found in process table => process has terminated, skip this line
  }
  else{
    result_proc->value->cur_index = handle_ftell_error(ftell(fp));
  }

  // ptr is the only page in ram list
  if (ptr == *ram_head && ptr == *ram_tail)
  {
    return;
  }

  // ptr is the tail of the ram list
  else if (ptr == *ram_tail && ptr != *ram_head)
  {
    return;
  }

  // ptr is at the head of ram list
  else if (ptr == *ram_head && ptr != *ram_tail)
  {
    // printf("move (%s %s, %ld) to last\n", ptr->pid, ptr->vpn, ptr->ppn);
    *ram_head = ptr->ram_next;
    (*ram_head)->ram_prev = NULL;
    (*ram_tail)->ram_next = ptr;
    ptr->ram_next = NULL;
    ptr->ram_prev = *ram_tail;
    *ram_tail = ptr;
  }

  else
  {
    // printf("move (%s %s, %ld) to last\n", ptr->pid, ptr->vpn, ptr->ppn);
    ptr->ram_prev->ram_next = ptr->ram_next;
    ptr->ram_next->ram_prev = ptr->ram_prev;
    (*ram_tail)->ram_next = ptr;
    ptr->ram_prev = *ram_tail;
    ptr->ram_next = NULL;
    *ram_tail = ptr;
  }
}