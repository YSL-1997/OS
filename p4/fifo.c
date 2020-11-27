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
*/
void fifo(process* process_head, process* process_tail, int num_pages)
{
  // page table and inverted page table
  struct hsearch_data* pt;
  struct hsearch_data* ipt;
  
  // head and tail pointers to the head of blocked processes
  process* io_head = NULL;
  process* io_tail = NULL;
  
  // head and tail pointers to the head of runnable processes
  process* runnable_head = process_head;
  process* runnable_tail = process_tail;
  
  // pointer to the head of free page frames
  page** free_arr = malloc_page_frames(num_pages);
  page* free_head = free_arr[0];
  page* free_tail = free_arr[1];
  
  // pointer to the head of the page frame that is in RAM
  page* ram_head = NULL;
  page* ram_tail = NULL;
  
  // global timer
  unsigned long global_timer = 0;

  FILE* fp = read_file("./tracefile");
  
  // the working buffer that stores each line of tracefile
  char* buf = (char*)malloc(sizeof(char) * MAX_LEN);
  handle_malloc_error(buf);

  /* 
     Important notes: fgets may return NULL for errors as well as EOF
     (EOF is not really an error). Thus, need to use feof() and ferror()
     to distinguish the cause of fgets failing.
  */
  do{
    // check if fp reaches the end of the tracefile
    if(feof(fp)){

      // if io_list is empty, and we've reached the end of file,
      // then all processes has terminated
      ////////////////////////////////////////////////////
      //////////////////////////////////////////////////////
      not sure!!!!
      if(io_head == NULL){
	break;
      }
      // nothing to read, if io_list is not empty, then wait for its completion
      if(io_head != NULL){
	wait_for_io_completion(&io_head, &io_tail,
			       &runnable_head, &runnable_tail,
			       *global_timer);
	///////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	need to do more
      }
    }
    
    if(fgets(buf, MAX_LEN, fp) != NULL){
      char** pid_vpn_pair = (char**)malloc(sizeof(char*) * 2);
      handle_malloc_error(pid_vpn_pair);

      // now the line is stored in buf, parse it
      pid_vpn_pair = parsing(buf);
      
      tmp_pid = pid_vpn_pair[0];
      tmp_vpn = pid_vpn_pair[1];

      // check if there are runnable processes
      if(runnable_head == NULL){
	// if no runnable process, then no need to read the rest of tracefile
	// all we need to do is to wait for I/O completion
	if(io_head != NULL){
	  wait_for_io_completion(&io_head, &io_tail,
				 &runnable_head, &runnable_tail,
				 *global_timer);
	  //////////////////////////////////////////////////
	  /////////////////////////////////////
	  need to do more
	}
      }
    }
    else{
      fprintf(stderr, "fgets() error\n");
      exit(EXIT_FAILURE);
    }
    
  }while();
    
}


/*
  initialize a single page frame
  input: ppn
  return: a pointer to a page struct
*/
page* initialize_page_frame(unsigned long ppn)
{
  // malloc the page frame
  page* page = (page*)malloc(sizeof(page));
  handle_malloc_error(page);

  // initialize the fields
  page->pid = 0;
  page->vpn = 0;
  page->ppn = ppn;
  page->ref_bit = 0;
  page->valid_bit = 0;
  page->pt_next = NULL;
  page->pt_prev = NULL;
  page->ipt_next = NULL;
  page->ipt_prev = NULL;
  page->free_next = NULL;
  page->free_prev = NULL;
  page->ram_next = NULL;
  page->ram_prev = NULL;

  return page;
}



/*
  malloc num_pages page frames
  input: number of page frames
  return: pointers to the head and tail of malloc'd page frames
          i.e. free_list and free_tail
*/
page** malloc_page_frames(unsigned long num_pages)
{
  page** ret = (page*)malloc(sizeof(page*) * 2);
  
  // malloc the first page frame
  page* page1 = initialize_page_frame(1);

  // create a pointer to the current page frame
  page* tmp = page1;
  
  for(int i = 0; i < num_pages-1; i++){
    // initialize the next page frame
    page* new_page = initialize_page_frame(i+2);

    // add the new page frame to the free_list
    tmp->free_next = new_page;
    new_page->free_prev = tmp;

    // set tmp to point to the new page
    tmp = new_page;
  }

  ret[0] = page1; // free_list
  ret[1] = tmp; // free_tail
  
  return ret;
}



/*
  this function pops from io_list
  input: head and tail pointers to I/O list
  return: pointer to the popped process struct
*/
process* pop_from_io(process** head, process** tail)
{
  if(*head == NULL && *tail == NULL){
    // if length of I/O list is 0
    return NULL;
  }
  else if(*head == *tail && *head != NULL){
    // if length of I/O list is 1
    process* tmp = *head;
    *head = NULL;
    *tail = NULL;
    return tmp;
  }
  else{
    // if length of I/O list is greater than 1
    process* tmp = *head;
    *head = (*head)->io_next;
    return tmp;
  }
}

void add_to_runnable(process* ptr, process** head, process** tail)
{
  if(*head == NULL && *tail == NULL){
    // runnable list is empty
    *head = ptr;
    *tail = ptr;
  }
  else{
    (*tail)->runnable_next = ptr;
    ptr->runnable_prev = *tail;
    *tail = ptr;
  }
}



/*  
  this function pops from io_list, update page struct, update timer, 
  add to ram_list,
  update page table, update inverted page table, update process table
  In order to enter this function, io_list must be non-empty
  input: all the pointers that keep track of info of simulator

*/
void wait_for_io_completion(process** io_head, process** io_tail,
			    process** runnable_head, process** runnable_tail,
			    *global_timer)
{
  process* tmp = pop_from_io(&io_head, &io_tail);
  tmp->is_blocked = false;
  add_to_runnable(tmp, &runnable_head, &runnable_tail);
  *global_timer += tmp->timer;
}



