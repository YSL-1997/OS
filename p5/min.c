/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

typedef struct node_bitmap
{
  int visited;
  page *ptr_pg;
} node_bitmap;

/* 
  MIN - page reference
  input: pointer to a page, process table and a file pointer
*/
void page_reference(page *ptr,
                    __attribute__((unused)) page **ram_head,
                    __attribute__((unused)) page **ram_tail,
                    void *proc_table, FILE *fp)
{
  node_proc *result_proc = find_proc(proc_table, ptr->pid);
  if (result_proc == NULL)
  {
    // not found in process table => process has terminated, skip this line
  }
  else
  {
    result_proc->value->cur_index = handle_ftell_error(ftell(fp));
  }
  return;
}

/*
  MIN - page replacement

  Note that, inside the functions, there are several lines that begin with
  printf, which is used for keeping track of the whole replacement process.
  We think that the printf()'s are really important for further use. That's
  why we did not delete them but comment them. 
  
  input: ram head, page table, tracefile, process table, frame size
  return: the page to be replaced
*/
page *page_replace(page **ram_head,
                   __attribute__((unused)) page **ram_tail,
                   __attribute__((unused)) page **clock_hand,
                   __attribute__((unused)) int *flag,
                   void *pt, char *filename, void **proc_table,
                   unsigned long frame_size)
{
  FILE *fp_min;                               // read trace file
  int MAX_LEN = 4096;                         // line max length
  char *buf = malloc(MAX_LEN * sizeof(char)); //store the line string

  char *cur_pid = NULL;
  char *cur_vpn = NULL;
  
  node_pt *result_pt = NULL; // store the page table search result

  fp_min = read_file(filename); // file pointer used in MIN Algorithm
  node_bitmap bitmap[frame_size]; // store the visited info for each page frame

  // initialize all pages with visited = 0
  for (unsigned long i = 0; i < frame_size; i++)
  {
    bitmap[i].visited = 0;
    bitmap[i].ptr_pg = NULL;
  }

  long max_ftell = 0;
  long max_index = 0; // bitmap[max_index] is the page to be replaced

  while (fgets(buf, MAX_LEN, fp_min) != NULL)
  {
    char **pid_vpn_pair = parsing(buf);

    cur_pid = pid_vpn_pair[0]; // current pid
    cur_vpn = pid_vpn_pair[1]; // current vpn

    node_proc *result_proc = find_proc(proc_table, cur_pid);
    
    // if pid is not in process table, i.e. the process has teminated
    if (result_proc == NULL)
    {
      free(cur_pid);
      free(cur_vpn);
      free(pid_vpn_pair);
      continue;
    }
    else
    {
      //check whether current location is less than the current index of
      // page, if so, the current trace has been executed, then skip.
      if (result_proc->value->cur_index >=
          (unsigned)handle_ftell_error(ftell(fp_min)))
      {
        // printf("(%s, %s) has been executed\n", cur_pid, cur_vpn);
        free(cur_pid);
        free(cur_vpn);
        free(pid_vpn_pair);
        continue;
      }
      else
      {
        // printf("(%s, %s) has not been executed\n", cur_pid, cur_vpn);
        char *key_pt = get_key_pt(cur_pid, cur_vpn);
        result_pt = find_pt(&pt, key_pt);

        // check whether the trace is in ram
        if (result_pt == NULL)
        {
          // printf("(%s, %s) is not in ram\n", cur_pid, cur_vpn);
          // if it is not in ram, skip this line
          free(cur_pid);
          free(cur_vpn);
          free(pid_vpn_pair);
          continue;
        }
        else
        {
          // printf("(%s, %s) is in ram\n", cur_pid, cur_vpn);
          if (bitmap[result_pt->value->ppn - 1].visited == 0)
          {
            // printf("(%s, %s) has not been visited is the latest one\n", 
            //        result_pt->value->pid, result_pt->value->vpn);
            // means this page has not been recorded yet
            bitmap[result_pt->value->ppn - 1].visited = 1;
            bitmap[result_pt->value->ppn - 1].ptr_pg = result_pt->value;

            // update the latest ppn and max read index
            if (max_ftell < handle_ftell_error(ftell(fp_min)))
            {
              max_ftell = handle_ftell_error(ftell(fp_min));
              max_index = result_pt->value->ppn - 1;
            }
          }

          free(cur_pid);
          free(cur_vpn);
          free(pid_vpn_pair);
          free(key_pt);
        }
      }
    }
  }

  // first go through the bitmap to see if there are pages that will no longer
  // be referenced, then evict that page
  for (unsigned long i = 0; i < frame_size; i++)
  {
    if (bitmap[i].visited == 0)
    {
      page *tmp = *ram_head;
      while (tmp->ppn != i + 1)
      {
        tmp = tmp->ram_next;
      }

      free(buf);
      fclose(fp_min);
      // printf("page ((%s, %s), %ld) replaced\n", tmp->pid, tmp->vpn, tmp->ppn);
      return tmp;
    }
  }

  // if all pages will be referenced in the future, then pick the latest one
  page *ret = bitmap[max_index].ptr_pg;
  free(buf);
  fclose(fp_min);
  // printf("page ((%s, %s), %ld) replaced\n", ret->pid, ret->vpn, ret->ppn);
  return ret;
}
