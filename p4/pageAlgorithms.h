#ifndef PAGEALGORITHMS_H_
#define PAGEALGORITHMS_H_

#define _GNU_SOURCE
#include <search.h>
#include "input.h"
#include "page.h"
#include "process.h"
#include "tsearch.h"
#include "assert.h"
#include "errorHandling.h"



page **malloc_page_frames(unsigned long num_pages);
process *pop_from_io(process **head, process **tail);
void add_to_runnable(process *ptr, process **head, process **tail);
char *get_key_pt(char *s1, char *s2);
void move_to_ram_tail(page **ram_head, page **ram_tail);
page *pop_from_free(page **free_head, page **free_tail);
void add_to_ram(page *ptr, page **head, page **tail);
void wait_for_io_completion(FILE **fp,
                            process **io_head, process **io_tail,
                            process **runnable_head, process **runnable_tail,
                            page **free_head, page **free_tail,
                            page **ram_head, page **ram_tail,
                            unsigned long *global_timer, void **pt, void **ipt);

process *remove_from_runnable(process *ptr, process **runnable_head,
                              process **runnable_tail);

void add_to_io(process *ptr, process **io_head, process **io_tail);
page *remove_from_ram(page *ptr, page **ram_head, page **ram_tail);
void add_to_free(page *ptr, page **free_head, page **free_tail);

void fifo(process **process_head, process **process_tail,
          unsigned long num_pages, void **proc_table);

#endif