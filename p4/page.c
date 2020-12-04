

#include "page.h"

/*
  initialize a single page frame
  input: ppn
  return: a pointer to a page struct
*/
page *initialize_page_frame(unsigned long ppn)
{
  // malloc the page frame
  page *new_page = (page *)malloc(sizeof(page));
  handle_malloc_error(new_page);

  // initialize the fields
  new_page->pid = "";
  new_page->vpn = "";
  new_page->ppn = ppn;
  new_page->ref_bit = 0;
  new_page->pt_next = NULL;
  new_page->pt_prev = NULL;
  new_page->ipt_next = NULL;
  new_page->ipt_prev = NULL;
  new_page->free_next = NULL;
  new_page->free_prev = NULL;
  new_page->ram_next = NULL;
  new_page->ram_prev = NULL;

  return new_page;
}