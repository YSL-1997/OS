/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

#include "pageAlgorithms.h"

/* 
  this function is the min replacement algorithm
  move the page from head to tail
  input: address of the ram_head, ram_tail, clock_hand and flag
  return: the pointer to the page to be replaced

*/
// page *page_replace1(page **ram_head, page **ram_tail,
//                     __attribute__((unused)) page **clock_hand,
//                     __attribute__((unused)) int *flag,
//                     void *pt, char *filename, void *proc_table)
// {
//   page *temp = *ram_head;
//   FILE *fp_min;                               // read trace file
//   int MAX_LEN = 4096;                         // line max length
//   char *buf = malloc(MAX_LEN * sizeof(char)); //store the line string

//   char *cur_pid = NULL;
//   char *cur_vpn = NULL;
//   // a bool flag to check whether the same pid, vpn pair is founded
//   bool found = false;

//   // set a latest offset value
//   long latest_offset = 0;
//   // variable to store the result page node in page table
//   node_pt *ret_node;

//   while (temp != NULL)
//   {
//     fp_min = read_file(filename);
//     // reset the variables

//     node_proc *result_proc = find_proc(proc_table, temp->pid);
//     // if not finding the pid in process table, means the process has teminated
//     // this may not happen, because we evict all pages related to terminated
//     //process from ram list
//     if (result_proc == NULL)
//     {
//       continue;
//     }

//     else
//     {
//       // get current index of the temp->pid, start scan from the current index
//       long offset = result_proc->value->cur_index;
//       fseek(fp_min, offset, SEEK_CUR);

//       while (fgets(buf, MAX_LEN, fp_min) != NULL)
//       {

//         char **pid_vpn_pair = parsing(buf);

//         cur_pid = pid_vpn_pair[0]; // current pid
//         cur_vpn = pid_vpn_pair[1]; // current vpn

//         // find the next trace with the same pid, vpn as the temp page in ram
//         if (strcmp(cur_pid, temp->pid) == 0 && strcmp(cur_vpn, temp->vpn) == 0)
//         {

//           found = true;
//           free(cur_pid);
//           free(cur_vpn);
//           free(pid_vpn_pair);
//           break;
//         }
//         else
//         {
//           free(cur_pid);
//           free(cur_vpn);
//           free(pid_vpn_pair);
//           continue;
//         }
//       }
//       // if the pid, vpn pair was not found in the forward trace, the temp page is
//       // the page that will be replaced
//       if (!found)
//       {
//         char *key_pt = get_key_pt(temp->pid, temp->vpn);
//         ret_node = find_pt(&pt, key_pt);
//         free(key_pt);
//         break;
//       }

//       // if found the next equal pid vpn piar, compare with the offset with the
//       // largest offset, if larger, reset largest offset and the return page node
//       else
//       {
//         if (handle_ftell_error(ftell(fp_min)) > latest_offset)
//         {
//           char *key_pt = get_key_pt(temp->pid, temp->vpn);
//           // free(cur_pid);
//           ret_node = find_pt(&pt, key_pt);
//           latest_offset = handle_ftell_error(ftell(fp_min));
//           free(key_pt);
//         }
//       }
//     }

//     temp = temp->ram_next;
//   }
//   free(buf);
//   return ret_node->value;
// }

/* 
  min page reference
  move the referenced page to the ram_tail
  input: pointer to a page, ram head, ram tail
*/
void page_reference(__attribute__((unused)) page *ptr,
                    __attribute__((unused)) page **ram_head,
                    __attribute__((unused)) page **ram_tail)
{
  return;
}

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

  node_pt *result_pt = NULL;
  ENTRY e, *ep;
  hcreate(frame_size);

  fp_min = read_file(filename);

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
      if (result_proc->value->cur_index >
          (unsigned)handle_ftell_error(ftell(fp_min)))
      {
        free(cur_pid);
        free(cur_vpn);
        free(pid_vpn_pair);
        continue;
      }
      else
      {
        char *key_pt = get_key_pt(cur_pid, cur_vpn);
        // free(cur_pid);
        result_pt = find_pt(&pt, key_pt);
        // free(key_pt);
        if (result_pt == NULL)
        {
          free(cur_pid);
          free(cur_vpn);
          free(pid_vpn_pair);
          continue;
        }
        else
        {
          e.key = key_pt;
          if (hsearch(e, FIND) == NULL)
          {
            e.data = (void *)handle_ftell_error(ftell(fp_min));
            hsearch(e, ENTER);
          }
          else
          {
            free(key_pt);
            continue;
          }
          free(cur_pid);
          free(cur_vpn);
          free(pid_vpn_pair);
        }
      }
    }
  }

  page *temp = *ram_head;
  long max_offset = 0;
  page *ret = NULL;
  while (temp != NULL)
  {
    e.key = get_key_pt(temp->pid, temp->vpn);
    ep = hsearch(e, FIND);
    if (ep == NULL)
    {
      ret = temp;
      free(e.key);
      break;
    }
    else
    {
      if ((long)ep->data > max_offset)
      {
        ret = temp;
        max_offset = (unsigned long)ep->data;
        free(e.key);
      }
    }
  }

  hdestroy();

  return ret;
}
