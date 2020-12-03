/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

/* 
  this function is the clock algorithm
  input: address of the ram_head, ram_tail, clock_hand and flag
  return: the pointer to the page to be replaced
  
  note that: flag is 0 => not yet initialized clock_hand
             flag is 1 => has initialized clock_hand
  
*/
page *page_replace(page **ram_head, page **ram_tail, page **clock_hand, int *flag)
{
  if (*flag == 0)
  {
    *clock_hand = *ram_head;
    *flag = 1;
  }

  assert(*clock_hand != NULL);

  while (*clock_hand != NULL)
  {
    if ((*clock_hand)->ref_bit == 0)
    {
      // this is the page to be replaced
      page *ret = *clock_hand;
      if ((*clock_hand)->ram_next == NULL)
      {
        *clock_hand = *ram_head;
      }
      else
      {
        *clock_hand = (*clock_hand)->ram_next;
      }
      return ret;
    }
    else
    {
      (*clock_hand)->ref_bit = 0;
      if ((*clock_hand)->ram_next == NULL)
      {
        *clock_hand = *ram_head;
      }
      else
      {
        *clock_hand = (*clock_hand)->ram_next;
      }
    }
  }
  return NULL;
}

/* 
  clock page reference
  move the referenced page to the ram_tail
*/
void page_reference(page *ptr, page **ram_head, page **ram_tail){
  ptr->ref_bit = 1;
  return;
}