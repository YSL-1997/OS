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
page *page_replace(page **ram_head, page **ram_tail, page **clock_hand, int *flag)
{
  page *ret = *ram_head;

  // move the head to tail
  move_to_ram_tail(ram_head, ram_tail);
  return ret;
}

/* 
  fifo page reference
  move the referenced page to the ram_tail
*/
void page_reference(page *ptr, page **ram_head, page **ram_tail){
  return;
}