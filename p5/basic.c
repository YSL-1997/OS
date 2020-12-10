/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "basic.h"

static int MAX_LEN = 4096;

/*
  read the tracefile and execute
  input: head of process list,
         tail of process list,
	       num of pages that can be allocated in RAM
         process table
*/
void basic(process **process_head, process **process_tail,
           unsigned long num_pages, void **proc_table, statistics *stat,
           char *filename)
{
  // page table
  void *pt = NULL;

  // head and tail pointers to the head of blocked processes
  process *io_head = NULL;
  process *io_tail = NULL;

  // head and tail pointers to the head of runnable processes
  process *runnable_head = *process_head;
  process *runnable_tail = *process_tail;

  // pointer to the head of free page frames
  page **free_arr = malloc_page_frames(num_pages);
  page *free_head = free_arr[0];
  page *free_tail = free_arr[1];

  // pointer to the head of the page frame that is in RAM
  page *ram_head = NULL;
  page *ram_tail = NULL;

  // used for clock algorithm
  page *clock_hand = NULL;
  int flag = 0;

  //--------------------------------------------------------------------
  // start processing the tracefile
  FILE *fp = read_file(filename);

  // the working buffer that stores each line of tracefile
  char *buf = (char *)malloc(sizeof(char) * MAX_LEN);
  handle_malloc_error(buf);

  // stores the current information of the line we've read
  char *cur_pid = NULL;
  char *cur_vpn = NULL;

  /*
     Important notes: fgets may return NULL for errors as well as EOF
     (EOF is not really an error). Thus, need to use feof() and ferror()
     to distinguish the cause of fgets failing.
  */
  do
  {
    // check if fp reaches the end of the tracefile
    if (feof(fp))
    {
      // if io_list is empty, and we've reached the end of file,
      // then all processes have terminated
      if (io_head == NULL)
      {
        break;
      }

      // if we've reached EOF & io_list is not empty,
      // then wait for its completion
      if (io_head != NULL)
      {
        wait_for_io_completion(&fp, &io_head, &io_tail,
                               &runnable_head, &runnable_tail,
                               &free_head, &free_tail,
                               &ram_head, &ram_tail,
                               stat, &pt, &clock_hand, &flag, filename, 
                               proc_table, num_pages);
        continue;
      }
    }

    // check if there are runnable processes
    if (runnable_head == NULL)
    {
      if (io_head != NULL)
      {
        // if no runnable process, then no need to read the rest of tracefile
        // all processes (if not all have terminated) are blocked in I/O list
        // all we need to do is to wait for I/O completion and start from there
        wait_for_io_completion(&fp, &io_head, &io_tail,
                               &runnable_head, &runnable_tail,
                               &free_head, &free_tail,
                               &ram_head, &ram_tail,
                               stat, &pt, &clock_hand, &flag, filename, 
                               proc_table, num_pages);

        continue;
      }
      else
      {
        // no runnable processes, no blocked processes
        break;
      }
    }

    // now, runnable list is not null (there are runnable processes)
    // read one line of the trace file at fp
    if (fgets(buf, MAX_LEN, fp) != NULL)
    {
      char **pid_vpn_pair = parsing(buf);

      cur_pid = pid_vpn_pair[0]; // current pid
      cur_vpn = pid_vpn_pair[1]; // current vpn

      // look into process table to see if it is in process table
      node_proc *result_proc = find_proc(proc_table, cur_pid);

      if (result_proc == NULL)
      {
        // not found in process table => process has terminated, skip this line
        free(cur_pid);
        free(cur_vpn);
        free(pid_vpn_pair);
        continue;
      }
      else
      {
        // found in the process table => process has not terminated
        if (result_proc->value->is_blocked)
        {
          // if this process is blocked, then it must be in io_queue, skip
          free(cur_pid);
          free(cur_vpn);
          free(pid_vpn_pair);
          continue;
        }
        else
        {
          // this process is not blocked, go to pt to see if there's page fault
          char *key_pt = get_key_pt(cur_pid, cur_vpn);
          free(cur_pid);
          node_pt *result_pt = find_pt(&pt, key_pt);
          free(key_pt);

          // not found, i.e. page fault
          if (result_pt == NULL)
          {
            // modify the stats
            stat->RTime += 1;
            stat->TPI += 1;
            stat->fake_AMU += stat->occupied_pages;
            stat->fake_ARP += stat->runnable_proc;

            // get the current position of fp
            long cur_pos = handle_ftell_error(ftell(fp));
            // modify the cur_index of this process
            result_proc->value->cur_index = cur_pos - strlen(buf);
            result_proc->value->is_blocked = true;
            result_proc->value->blocked_vpn = cur_vpn;
            result_proc->value->timer = 2000000;
            result_proc->value->io_next = NULL;

            // check if io list is currently empty
            bool io_empty = true;
            if (io_head != NULL && io_tail != NULL)
            {
              io_empty = false;
            }

            // remove from runnable list
            process *popped_proc = remove_from_runnable(result_proc->value,
                                                        &runnable_head,
                                                        &runnable_tail);
            // numbwe of running process decrease by 1
            stat->runnable_proc -= 1;
            // add to io list
            add_to_io(popped_proc, &io_head, &io_tail);

            // before add_to_io, io list was not empty
            if (io_head->io_next != NULL && !io_empty)
            {
              // when a clock tick, the waiting time for the head of I/O list
              //decreases by 1
              io_head->timer -= 1;
              // when the I/O list head completes, set the structures
              if (io_head->timer == 0)
              {
                wait_for_io_completion(&fp, &io_head, &io_tail,
                                       &runnable_head, &runnable_tail,
                                       &free_head, &free_tail,
                                       &ram_head, &ram_tail,
                                       stat, &pt, &clock_hand, &flag, filename, 
                                       proc_table, num_pages);
                continue;
              }
            }
          }

          // if page exists in memory
          else
          {
            //check whether current location is less than the current index of
            // page, if so, the current trace has been implemented, then skip.
            if (result_proc->value->cur_index >
                (unsigned)handle_ftell_error(ftell(fp)))
            {
              continue;
            }

            free(cur_vpn);
            // reference the page
            page_reference(result_pt->value, &ram_head, &ram_tail);

            // update the stats
            stat->RTime += 1;
            stat->TMR += 1;
            stat->fake_AMU += stat->occupied_pages;
            stat->fake_ARP += stat->runnable_proc;

            // check if this process has terminated
            // result_proc->value is pointer to this process
            if ((unsigned)handle_ftell_error(ftell(fp)) ==
                result_proc->value->end_index)
            {
              // this process has terminated
              // remove this process from runnable list
              process *end_proc = remove_from_runnable(result_proc->value,
                                                       &runnable_head,
                                                       &runnable_tail);
              stat->runnable_proc -= 1;
              // end_proc->pid, this entry needs to be deleted in proc_table
              delete_proc(proc_table, end_proc->pid);

              // need to remove the corresponding entries in pt, ipt
              // remove the page frames that corresponds to end_proc
              page *tmp = ram_head;
              while (tmp != NULL)
              {
                page *tmp2 = tmp->ram_next;

                // if (!strncmp(tmp->pid, end_proc->pid, max))  is evil!
                if (strcmp(tmp->pid, end_proc->pid) == 0)
                {
                  // need to remove tmp from ram_list, add to free_list
                  // note that we do not modify anything stored in the page tmp
                  page *removed_page = remove_from_ram(tmp, &ram_head,
                                                       &ram_tail);
                  stat->occupied_pages -= 1;

                  add_to_free(removed_page, &free_head, &free_tail);

                  // remove the entry in page table
                  char *key_pt_del = get_key_pt(removed_page->pid,
                                                removed_page->vpn);
                  delete_pt(&pt, key_pt_del);
                  free(key_pt_del);
                  free(removed_page->vpn);
                }
                tmp = tmp2;
              }
              free(end_proc->pid);
              free(end_proc);
            }

            // check if io list is empty
            bool io_empty = true;
            if (io_head != NULL && io_tail != NULL)
            {
              io_empty = false;
            }

            if (!io_empty)
            {
              io_head->timer -= 1;

              if (io_head->timer == 0)
              {
                wait_for_io_completion(&fp, &io_head, &io_tail,
                                       &runnable_head, &runnable_tail,
                                       &free_head, &free_tail,
                                       &ram_head, &ram_tail,
                                       stat, &pt, &clock_hand, &flag, filename, 
                                       proc_table, num_pages);
                continue;
              }
            }
          }
        }
      }
      free(pid_vpn_pair);
    }
    
  } while (true);
  free(buf);
  while(free_head != NULL){
    page* tmp = free_head->free_next;
    free(free_head);
    free_head = tmp;
  }
  free(free_arr);
}

/*
  malloc num_pages page frames
  input: number of page frames
  return: pointers to the head and tail of malloc'd page frames
          i.e. free_list and free_tail
*/
page **malloc_page_frames(unsigned long num_pages)
{
  page **ret = (page **)malloc(sizeof(page *) * 2);
  handle_malloc_error(ret);

  // malloc the first page frame (unsigned long ppn = 1)
  page *page1 = initialize_page_frame(1);

  // create a pointer to the current page frame
  page *tmp = page1;

  for (unsigned long i = 0; i < num_pages - 1; i++)
  {
    // initialize the next page frame
    page *new_page = initialize_page_frame(i + 2);

    // add the new page frame to the free_list
    tmp->free_next = new_page;
    new_page->free_prev = tmp;

    // set tmp to point to the new page
    tmp = new_page;
  }

  ret[0] = page1; // free_head
  ret[1] = tmp;   // free_tail

  return ret;
}

/*
  this function pops from io_list
  input: head and tail pointers to I/O list
  return: pointer to the popped process struct
*/
process *pop_from_io(process **head, process **tail)
{
  // printf("popped from io: pid=%s\n", (*head)->pid);
  if (*head == NULL && *tail == NULL)
  {
    // if length of I/O list is 0
    return NULL;
  }
  else if (*head == *tail && *head != NULL)
  {
    // if length of I/O list is 1
    process *tmp = *head;
    *head = NULL;
    *tail = NULL;
    return tmp;
  }
  else
  {
    // if length of I/O list is greater than 1
    process *tmp = *head;
    *head = (*head)->io_next;
    return tmp;
  }
}

/*
  this function adds a process to the tail of io list
  input: pointer to the to-be-added process, io_head, io_tail
*/
void add_to_io(process *ptr, process **io_head, process **io_tail)
{
  // printf("add to io queue: pid=%s\n", ptr->pid);
  if (*io_head == *io_tail && *io_head == NULL)
  {
    // io list is empty
    *io_head = ptr;
    *io_tail = ptr;
    ptr->io_next = NULL;
  }
  else
  {
    // io list has at least one processes
    (*io_tail)->io_next = ptr;
    *io_tail = ptr;
    ptr->io_next = NULL;
  }
}

/*
  this function adds a process popped from io_queue to runnable queue
  input: a pointer to a process struct, runnable_head, runnable_tail
*/
void add_to_runnable(process *ptr, process **head, process **tail)
{
  if (*head == NULL && *tail == NULL)
  {
    // runnable list is empty
    *head = ptr;
    *tail = ptr;
    ptr->runnable_next = NULL;
    ptr->runnable_prev = NULL;
  }
  else
  {
    (*tail)->runnable_next = ptr;
    ptr->runnable_prev = *tail;
    *tail = ptr;
    (*tail)->runnable_next = NULL;
  }
}

/*
  get the concatenated key string from a pid and a vpn
  input: pointer to a page struct
  return: key string
*/
char *get_key_pt(char *s1, char *s2)
{
  char *key_str = (char *)malloc(sizeof(char) * (strlen(s1) +
                                                 strlen(s2) + 2));
  handle_malloc_error(key_str);

  snprintf(key_str, strlen(s1) + strlen(s2) + 2, "%s %s", s1, s2);
  return key_str;
}

/*
  in FIFO, move the head of ram_queue to the tail
  input: two pointers to ram_head and ram_tail
*/
void move_to_ram_tail(page **ram_head, page **ram_tail)
{
  if (*ram_head == *ram_tail && *ram_head != NULL)
  {
    // do nothing
  }
  if (*ram_head != *ram_tail)
  {
    page *tmp = *ram_head;
    *ram_head = tmp->ram_next;
    (*ram_head)->ram_prev = NULL;
    (*ram_tail)->ram_next = tmp;
    tmp->ram_prev = *ram_tail;
    *ram_tail = tmp;
    tmp->ram_next = NULL;
  }
}

/*
  this function pops from free_list
  input: head and tail pointers to free list
  return: pointer to the popped page struct
*/
page *pop_from_free(page **free_head, page **free_tail)
{
  if (*free_head == NULL && *free_tail == NULL)
  {
    return NULL;
  }
  else if (*free_head == *free_tail && *free_head != NULL)
  {
    // if length of free list is 1
    page *tmp = *free_head;
    *free_head = NULL;
    *free_tail = NULL;
    return tmp;
  }
  else
  {
    // if length of free list is greater than 1
    page *tmp = *free_head;
    *free_head = (*free_head)->free_next;
    (*free_head)->free_prev = NULL;
    tmp->free_next = NULL;
    tmp->free_prev = NULL;
    return tmp;
  }
}

/*
  this function adds a page popped from free list to ram list
  input: a pointer to a page struct, ram_head, ram_tail
*/
void add_to_ram(page *ptr, page **head, page **tail)
{
  if (*head == NULL && *tail == NULL)
  {
    // ram list is empty
    *head = ptr;
    *tail = ptr;
  }
  else
  {
    (*tail)->ram_next = ptr;
    ptr->ram_prev = *tail;
    *tail = ptr;
    ptr->ram_next = NULL;
  }
}

/*
  this function pops from io_list, update timer, add to runnable list,
  update page table, inverted page table, free list (if needed), ram list,
  and set file pointer to the blocked index when page fault occurred

  after executing this function, the desired page has been fetched from disk
  to RAM already, with pt, ipt updated.

  in order to enter this function, io_list must be non-empty
  input: all the pointers that keep track of info of simulator
*/
void wait_for_io_completion(FILE **fp,
                            process **io_head, process **io_tail,
                            process **runnable_head, process **runnable_tail,
                            page **free_head, page **free_tail,
                            page **ram_head, page **ram_tail,
                            statistics *stat, void **pt,
                            page **clock_hand, int *flag, char *filename, 
                            void **proc_table, unsigned long num_pages)
{
  process *tmp = pop_from_io(io_head, io_tail);

  stat->RTime += tmp->timer;
  stat->fake_AMU += stat->occupied_pages * tmp->timer;
  stat->fake_ARP += stat->runnable_proc * tmp->timer;
  // below is the original approach, which was very slow
  // for (int i = 0; i < tmp->timer; i++)
  // {
  //   stat->fake_AMU += stat->occupied_pages;
  //   stat->fake_ARP += stat->runnable_proc;
  //   stat->RTime += 1;
  // }

  add_to_runnable(tmp, runnable_head, runnable_tail);
  stat->runnable_proc += 1;

  // get the page to replace
  if (*free_head == NULL)
  {
    // need to replace a page

    // this function moves the page to ram_tail if needed (clock does not)
    // this function returns the page to be replaced
    page *page_to_replace = page_replace(ram_head, ram_tail, clock_hand, flag,
                                        *pt, filename, proc_table, num_pages);

    // get the key of pt to be deleted
    // update pt (delete former entry)
    char *key_pt = get_key_pt(page_to_replace->pid, page_to_replace->vpn);
    delete_pt(pt, key_pt);
    free(key_pt);
    free(page_to_replace->vpn);

    // modify the page
    page_to_replace->pid = (*runnable_tail)->pid;
    page_to_replace->vpn = (*runnable_tail)->blocked_vpn;

    // add the "new" page to the page table
    node_pt *entry_pt = create_entry_pt(page_to_replace);
    add_to_pt(pt, entry_pt);
  }
  else
  {
    // no need to replace a page
    // take free_head as the page to allocate
    page *page_to_replace = pop_from_free(free_head, free_tail);
    page_to_replace->pid = (*runnable_tail)->pid;
    page_to_replace->vpn = (*runnable_tail)->blocked_vpn;

    add_to_ram(page_to_replace, ram_head, ram_tail);
    stat->occupied_pages += 1;

    // update page table
    // since no page replacement, simply add the entry to pt
    node_pt *entry_pt = create_entry_pt(*ram_tail); // create entry
    add_to_pt(pt, entry_pt);
  }

  // now, I/O completes, go to execute the previously blocked process
  (*runnable_tail)->is_blocked = false;

  // get the current position of fp
  long cur_pos = handle_ftell_error(ftell(*fp));

  // get the offset that passed into fseek()
  long offset = (*runnable_tail)->cur_index - cur_pos;

  // modify fp
  fseek(*fp, offset, SEEK_CUR);
}

/*
  this function removes a process from the runnable list
  input: pointer to the process to be removed, runnable_head, runnable_tail
  return: pointer to the removed process
*/
process *remove_from_runnable(process *ptr, process **runnable_head,
                              process **runnable_tail)
{
  assert(*runnable_head != NULL);
  assert(*runnable_tail != NULL);
  if (*runnable_head == ptr && *runnable_tail == ptr)
  {
    *runnable_head = NULL;
    *runnable_tail = NULL;
    return ptr;
  }
  else if (*runnable_head == ptr && *runnable_tail != ptr)
  {
    // ptr is the head of runnable list
    *runnable_head = ptr->runnable_next;
    ptr->runnable_next->runnable_prev = NULL;
    ptr->runnable_next = NULL;
    return ptr;
  }
  else if (*runnable_tail == ptr && *runnable_head != ptr)
  {
    // ptr is the tail of runnable list
    *runnable_tail = ptr->runnable_prev;
    ptr->runnable_prev->runnable_next = NULL;
    ptr->runnable_prev = NULL;
    return ptr;
  }
  else
  {
    ptr->runnable_prev->runnable_next = ptr->runnable_next;
    ptr->runnable_next->runnable_prev = ptr->runnable_prev;
    ptr->runnable_next = NULL;
    ptr->runnable_prev = NULL;
    return ptr;
  }
}

/*
  this function removes a page frame from ram list
  input: a pointer to a page, ram head and ram tail
  return: the pointer to the page that removed
*/
page *remove_from_ram(page *ptr, page **ram_head, page **ram_tail)
{

  // if ram list is null, report an error
  if (*ram_head == NULL && *ram_tail == NULL)
  {
    fprintf(stderr, "ram list is empty\n");
    exit(1);
  }

  // if length of ram list is 1, i.e. only ptr is in ram list
  else if (*ram_head == ptr && *ram_tail == ptr)
  {
    *ram_head = NULL;
    *ram_tail = NULL;
    return ptr;
  }

  else if (*ram_head == ptr && *ram_tail != ptr)
  {
    // ptr is the head of ram list
    *ram_head = ptr->ram_next;
    ptr->ram_next->ram_prev = NULL;
    ptr->ram_next = NULL;
    return ptr;
  }

  else if (*ram_tail == ptr && *ram_head != ptr)
  {
    // ptr is the tail of runnable list
    *ram_tail = ptr->ram_prev;
    ptr->ram_prev->ram_next = NULL;
    ptr->ram_prev = NULL;
    return ptr;
  }
  else
  {
    ptr->ram_prev->ram_next = ptr->ram_next;
    ptr->ram_next->ram_prev = ptr->ram_prev;
    ptr->ram_next = NULL;
    ptr->ram_prev = NULL;
    return ptr;
  }
}

/*
  this function adds the page to the tail of the free list
  input: pointer to the page frame, free head, free tail
*/
void add_to_free(page *ptr, page **free_head, page **free_tail)
{

  if (*free_head == *free_tail && *free_head == NULL)
  {
    // free list is empty
    *free_head = ptr;
    *free_tail = ptr;
  }
  else
  {
    // free list has at least one page frame
    (*free_tail)->free_next = ptr;
    *free_tail = ptr;
    ptr->free_next = NULL;
    ptr->free_prev = NULL;
  }
}

