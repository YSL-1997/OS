/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

static int MAX_LEN = 4096;

/*
  read the tracefile and execute
  input: head of process list,
         tail of process list,
	       num of pages that can be allocated in RAM
         process table
*/
void fifo(process **process_head, process **process_tail,
          unsigned long num_pages, void **proc_table)
{
  // page table and inverted page table
  void *pt;
  void *ipt;

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

  // global timer
  unsigned long global_timer = 0;

  //--------------------------------------------------------------------
  // start processing the tracefile
  FILE *fp = read_file("./tracefile.txt");

  // the working buffer that stores each line of tracefile
  char *buf = (char *)malloc(sizeof(char) * MAX_LEN);
  handle_malloc_error(buf);

  // stores the current information of the line we've read
  char *cur_pid = "";
  char *cur_vpn = "";

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
        exit(0);
      }

      // if we've reached EOF & io_list is not empty,
      // then wait for its completion
      if (io_head != NULL)
      {
        wait_for_io_completion(&fp, &io_head, &io_tail,
                               &runnable_head, &runnable_tail,
                               &free_head, &free_tail,
                               &ram_head, &ram_tail,
                               &global_timer, &pt, &ipt);
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
                               &global_timer, &pt, &ipt);

        continue;
      }
      else
      {
        // io_head == NULL
        // no runnable processes, no blocked processes
        exit(0);
      }
    }

    // now, runnable list is not null (there are runnable processes)
    // read one line of the trace file at fp
    if (fgets(buf, MAX_LEN, fp) != NULL)
    {
      char **pid_vpn_pair = (char **)malloc(sizeof(char *) * 2);
      handle_malloc_error(pid_vpn_pair);

      // now the line is stored in buf, parse it
      pid_vpn_pair = parsing(buf);

      cur_pid = pid_vpn_pair[0]; // current pid
      cur_vpn = pid_vpn_pair[1]; // current vpn

      // look into process table to see if it is in process table
      node_proc *result_proc = find_proc(proc_table, cur_pid);

      if (result_proc == NULL)
      {
        // not found in process table => process has terminated, skip this line
        continue;
      }
      else
      {
        // found in the process table => process has not terminated
        if (result_proc->value->is_blocked)
        {
          // if this process is blocked, then it must be in io_queue, skip
          continue;
        }
        else
        {
          // this process is not blocked, go to pt to see if there's page fault
          char *key_pt = get_key_pt(cur_pid, cur_vpn);
          node_pt *result_pt = find_pt(&pt, key_pt);

          if (result_pt == NULL)
          {
            /* not found, i.e. page fault
            // (+1 ns)（io_head timer -1, 检查是否为0 if 0：go to "x"）
            // 回溯file pointer，然后更新process的blocked_index (即回溯后的fp), blocked vpn,
            // isBlocked = true, 将process从runnable list移除，加入io queue (2000000)
            // "x": wait_for_io_completion()
            */

            // get the current position of fp
            long cur_pos = ftell(fp);
            // modify the cur_index of this process
            result_proc->value->cur_index = cur_pos - strlen(buf);
            result_proc->value->is_blocked = true;
            result_proc->value->blocked_vpn = cur_vpn;
            result_proc->value->timer = 2000000;
            result_proc->value->io_next = NULL;

            // check if io list is empty
            bool io_empty = true;
            if (io_head != NULL && io_tail != NULL)
            {
              io_empty = false;
            }

            // remove from runnable list
            process *popped_proc = remove_from_runnable(result_proc->value,
                                                        &runnable_head,
                                                        &runnable_tail);

            // add to io list
            add_to_io(popped_proc, &io_head, &io_tail);

            // modify the timer
            global_timer++;

            if (io_head->io_next != NULL && !io_empty)
            {
              // before add_to_io, io list was not empty
              io_head->timer--;

              if (io_head->timer == 0)
              {
                wait_for_io_completion(&fp, &io_head, &io_tail,
                                       &runnable_head, &runnable_tail,
                                       &free_head, &free_tail,
                                       &ram_head, &ram_tail,
                                       &global_timer, &pt, &ipt);
                continue;
              }
            }
          }
          else
          {
            // found, no page fault, just reference the page (+1 ns)
            // 如果有找到，就reference （+1 ns）（io_head timer -1, 检查是否为0 if 0：go to "x"）
            // 在reference之后，检查是否这个process已经达到了end index
            //  if 到达end index，就remove 这个process （runnable list中），以及对应的page from pt(a lot) ipt(a lot)
            // "x": wait_for_io_completion()

            global_timer++; // reference the page

            // 在reference之后，检查是否这个process已经达到了end index
            // check if this process has terminated
            // result_proc->value is pointer to this process
            if (ftell(fp) == result_proc->value->end_index)
            {
              //  if 到达end index，就remove 这个process （runnable list中），
              // if this process has terminated
              // remove this process from runnable list
              process *end_proc = remove_from_runnable(result_proc->value,
                                                       &runnable_head,
                                                       &runnable_tail);
              // end_proc->pid, this entry needs to be deleted in proc_table
              delete_proc(proc_table, end_proc->pid);

              // 以及对应的page from pt(a lot) ipt(a lot)
              // need to remove the corresponding entries in pt, ipt
              // remove the page frames that corresponds to end_proc
              page *tmp = ram_head;
              while (tmp != NULL)
              {

                // if we find the page that corresponds to end_proc
                if (!strncmp(tmp->pid, end_proc->pid, strlen(tmp->pid)))
                {
                  // need to remove tmp from ram_list, add to free_list
                  // note that we do not modify anything stored in the page tmp
                  page *removed_page = remove_from_ram(tmp, &ram_head, &ram_tail);
                  add_to_free(removed_page, &free_head, &free_tail);

                  // remove the entry in pt and ipt
                  delete_pt(&pt, get_key_pt(tmp->pid, tmp->vpn));
                  delete_ipt(&ipt, tmp->ppn);
                }
                tmp = tmp->ram_next;
              }

              free(end_proc);
            }

            // （io_head timer -1, 检查是否为0 if 0：go to "x"）
            // check if io list is empty
            bool io_empty = true;
            if (io_head != NULL && io_tail != NULL)
            {
              io_empty = false;
            }

            if (!io_empty)
            {
              io_head->timer--;

              if (io_head->timer == 0)
              {
                wait_for_io_completion(&fp, &io_head, &io_tail,
                                       &runnable_head, &runnable_tail,
                                       &free_head, &free_tail,
                                       &ram_head, &ram_tail,
                                       &global_timer, &pt, &ipt);
                continue;
              }
            }
          }

          // fgets之后，如果当前行可以执行，memory reference 之后，fp == end index。说明刚刚执行的process结束了，要free掉它
          //  即：从runnable list里删除，从process table里删除，然后：traverse ram list，将所有当前process
          //  对应的page 移动到 free list中 （因为我们traverse page，所以可以得到pid,vpn,ppn。所以
          //  我们可以同时将"pid vpn" 和ppn变成key，删除对应的pt entry，ipt entry）
        }
      }
    }
    else
    {
      fprintf(stderr, "fgets() error\n");
      exit(EXIT_FAILURE);
    }

  } while (true);
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

  // malloc the first page frame (unsigned long ppn = 1)
  page *page1 = initialize_page_frame(1);

  // create a pointer to the current page frame
  page *tmp = page1;

  for (int i = 0; i < num_pages - 1; i++)
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
  get the concatenated key string from a page struct ("pid vpn")
  input: pointer to a page struct
  return: key string
*/
// char *get_key_pt(page *ptr)
// {
//   char *key_str = (char *)malloc(sizeof(char) * (strlen(ptr->pid) +
//                                                  strlen(ptr->vpn) + 2));
//   handle_malloc_error(key_str);

//   // strcat(key_str, ptr->pid);
//   // strcat(key_str, " ");
//   // strcat(key_str, ptr->vpn);
//   snprintf(key_str, strlen(ptr->pid) + strlen(ptr->vpn) + 2,
//            "%s %s", ptr->pid, ptr->vpn);
//   return key_str;
// }
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

  // strcat(key_str, s1);
  // strcat(key_str, " ");
  // strcat(key_str, s2);
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
                            unsigned long *global_timer, void **pt, void **ipt)
{
  process *tmp = pop_from_io(io_head, io_tail);
  add_to_runnable(tmp, runnable_head, runnable_tail);
  *global_timer += tmp->timer;

  // specific to FIFO, get the page to replace-------------
  if (*free_head == NULL)
  {
    // need to replace a page
    // page_to_replace is ram_head;

    // update pt (delete former entry)
    // get the key of pt to be deleted
    char *key_pt = get_key_pt((*ram_head)->pid, (*ram_head)->vpn);

    delete_pt(pt, key_pt);

    // modify the page
    (*ram_head)->pid = (*runnable_tail)->pid;
    (*ram_head)->vpn = (*runnable_tail)->blocked_vpn;
    // move the page from ram_head to ram_tail
    move_to_ram_tail(ram_head, ram_tail);

    // update ipt (no need in this case)

    // add the "new" page to the page table
    node_pt *entry_pt = create_entry_pt(*ram_tail); // create entry
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

    // update page table
    // since no page replacement, simply add the entry to pt
    node_pt *entry_pt = create_entry_pt(*ram_tail); // create entry
    add_to_pt(pt, entry_pt);

    // add to ipt
    node_ipt *entry_ipt = create_entry_ipt((*ram_tail)->ppn, *ram_tail);
    add_to_ipt(ipt, entry_ipt);
  }

  // now, I/O completes, go to execute the previously blocked process
  (*runnable_tail)->is_blocked = false;

  // get the current position of fp
  long cur_pos = ftell(*fp);

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
  this function adds a process to the tail of io list
  input: pointer to the to-be-added process, io_head, io_tail
*/
void add_to_io(process *ptr, process **io_head, process **io_tail)
{
  if (*io_head == *io_tail && *io_head == NULL)
  {
    // io list is empty
    *io_head = ptr;
    *io_tail = ptr;
  }
  else
  {
    // io list has at least one processes
    (*io_tail)->io_next = ptr;
    *io_tail = ptr;
    ptr->io_next = NULL;
  }
}

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