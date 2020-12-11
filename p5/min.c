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
  min page reference
  input: pointer to a page, ram head, ram tail
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

page *page_replace(page **ram_head,
                   __attribute__((unused)) page **ram_tail,
                   __attribute__((unused)) page **clock_hand,
                   __attribute__((unused)) int *flag,
                   void *pt, char *filename, void **proc_table,
                   unsigned long frame_size)
{
  printf("hello\n");
  FILE *fp_min;                               // read trace file
  int MAX_LEN = 4096;                         // line max length
  char *buf = malloc(MAX_LEN * sizeof(char)); //store the line string

  char *cur_pid = NULL;
  char *cur_vpn = NULL;
  // to store the page table search result
  node_pt *result_pt = NULL;

  // create a hash table to store the <pid, vpn> pair as key and the offset of
  // fist trace with pid, vpn existing in the ram page as value
  // ENTRY e, *ep;
  // hcreate(frame_size);

  // create a string array to store the hashtable key, it will be used to free
  // the key of hashtable
  // char *key_array[frame_size];
  // unsigned long key_index = 0;

  fp_min = read_file(filename);
  node_bitmap bitmap[frame_size];
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
    // if not finding the pid in process table, means the process has teminated
    // this may not happen, because we evict all pages related to terminated
    //process from ram list
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
      // page, if so, the current trace has been implemented, then skip.
      if (result_proc->value->cur_index >=
          (unsigned)handle_ftell_error(ftell(fp_min)))
      {
        printf("(%s, %s) has been executed\n", cur_pid, cur_vpn);
        free(cur_pid);
        free(cur_vpn);
        free(pid_vpn_pair);
        continue;
      }
      else
      {
        printf("(%s, %s) has not been executed\n", cur_pid, cur_vpn);
        // printf("in: %s\n", cur_pid);
        char *key_pt = get_key_pt(cur_pid, cur_vpn);
        result_pt = find_pt(&pt, key_pt);

        // check whether the trace is in ram
        if (result_pt == NULL)
        {
          printf("(%s, %s) is not in ram\n", cur_pid, cur_vpn);
          // if it is not in ram, skip this line
          free(cur_pid);
          free(cur_vpn);
          free(pid_vpn_pair);
          continue;
        }
        else
        {
          // e.key = key_pt;
          // // if the trace was in ram but not in the hashtable, that means the
          // // trace is one of the first pid, vpn existing in ram but not be hitted
          // if (hsearch(e, FIND) == NULL)
          // {
          //   e.data = (void *)handle_ftell_error(ftell(fp_min));
          //   hsearch(e, ENTER);

          //   // store the key pointer of the hash table
          //   key_array[key_index] =  e.key;
          //   key_index++;

          //   free(cur_pid);
          //   free(cur_vpn);
          //   free(pid_vpn_pair);
          // }
          // // if the pid, vpn were in hashtable, that means it is not the first
          // // trace not be  read but exists in ram
          // else
          // {
          //   free(key_pt);
          //   free(cur_pid);
          //   free(cur_vpn);
          //   free(pid_vpn_pair);
          //   continue;
          // }
          printf("(%s, %s) is in ram\n", cur_pid, cur_vpn);
          if (bitmap[result_pt->value->ppn - 1].visited == 0)
          {
            printf("(%s, %s) has not been visited is the latest one\n", result_pt->value->pid, result_pt->value->vpn);
            // means this page has not been recorded yet
            bitmap[result_pt->value->ppn - 1].visited = 1;
            bitmap[result_pt->value->ppn - 1].ptr_pg = result_pt->value;

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

  // page *temp = *ram_head;
  // long max_offset = 0;
  // page *ret = NULL;
  // // loop the ram list/ the hashtable
  // while (temp != NULL)
  // {
  //   e.key = get_key_pt(temp->pid, temp->vpn);
  //   ep = hsearch(e, FIND);
  //   // if the ram page were not in hashtable, that means there is no trace with
  //   //same pid, vpn as the page, so the page can be replaced
  //   if (ep == NULL)
  //   {
  //     ret = temp;
  //     free(e.key);
  //     break;
  //   }
  //   // if the ram page were in hashtable, compare the offset of the same trace
  //   // and if it was larger than the current max, reset return page and max
  //   else
  //   {
  //     if ((long)ep->data > max_offset)
  //     {
  //       ret = temp;
  //       max_offset = (unsigned long)ep->data;
  //     }
  //     free(e.key);
  //   }
  //   temp = temp->ram_next;
  // }

  // hdestroy();

  // for(unsigned long i = 0; i < key_index; i++){
  //   free(key_array[i]);
  // }

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
      printf("page ((%s, %s), %ld) replaced\n", tmp->pid, tmp->vpn, tmp->ppn);
      return tmp;
    }
  }

  page *ret = bitmap[max_index].ptr_pg;
  free(buf);
  fclose(fp_min);
  printf("page ((%s, %s), %ld) replaced\n", ret->pid, ret->vpn, ret->ppn);
  return ret;
}
