#ifndef PAGEALGORITHMS_H_
#define PAGEALGORITHMS_H_

#include "page.h"
#include "assert.h"

/*
  this is the api for the three page replacement algorithms
  after calling this function, the page frame has been moved but not yet updated
  (e.g. in FIFO and LRU, the first page has been moved to the last
        in Clock, just move the clock_hand and find the page to be replaced)
  input:
  FIFO and LRU only operates on the first two parameters: ram_head and ram_tail
  Clock requires the last two params, with the clock_hand and the flag that 
  indicates whether the clock_hand has been initialized or not.

  return:
  a pointer to a page to be replaced
*/
page *page_replace(page **ram_head, page **ram_tail, page **clock_hand,
                   int *flag);

/*
  this is the api for referencing a page for three algorithms
  after calling this function:
    FIFO: do nothing
    LRU: moves the page to the ram tail
    Clock: set the ref_bit of the page to be 0
    
  input: a pointer to a page to be referenced, ram head and ram tail 
*/
void page_reference(page *ptr, page **ram_head, page **ram_tail);

void move_to_ram_tail(page **ram_head, page **ram_tail);

#endif