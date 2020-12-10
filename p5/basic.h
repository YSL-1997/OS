#ifndef BASIC_H_
#define BASIC_H_

#define _GNU_SOURCE
#include <search.h>
#include "input.h"
#include "page.h"
#include "process.h"
#include "tsearch.h"
#include "errorHandling.h"
#include "statistics.h"
#include "pageAlgorithms.h"

page **malloc_page_frames(unsigned long num_pages);
char *get_key_pt(char *s1, char *s2);

// below functions are implemented differently based on our implementation
// this is because struct page includes all the pointers used in 4 queues
process *pop_from_io(process **head, process **tail);
page *pop_from_free(page **free_head, page **free_tail);
page *remove_from_ram(page *ptr, page **ram_head, page **ram_tail);
process *remove_from_runnable(process *ptr, process **runnable_head,
                              process **runnable_tail);

void add_to_io(process *ptr, process **io_head, process **io_tail);
void add_to_free(page *ptr, page **free_head, page **free_tail);
void add_to_ram(page *ptr, page **head, page **tail);
void add_to_runnable(process *ptr, process **head, process **tail);

void wait_for_io_completion(FILE **fp,
                            process **io_head, process **io_tail,
                            process **runnable_head, process **runnable_tail,
                            page **free_head, page **free_tail,
                            page **ram_head, page **ram_tail,
                            statistics *stat, void **pt,
                            page **clock_hand, int *flag, char *filename, 
                            void **proc_table, unsigned long frame_size);

void basic(process **process_head, process **process_tail,
           unsigned long num_pages, void **proc_table, statistics *stat,
           char *filename);


#endif