/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

/*
  This module is cited from:
  https://github.com/increscent/c_examples/blob/main/tsearch/main.c and
  https://nixdoc.net/man-pages/Linux/man3/tsearch.3.html#EXAMPLE
*/

#include "tsearch.h"

/*
  this function creates an entry of the process table
  input: pid, pointer to the process struct
  return: a pointer to the node_proc struct
*/
node_proc *create_entry_proc(char *pid, process *proc)
{
  node_proc *new_proc = malloc(sizeof(node_proc));
  handle_malloc_error(new_proc);

  new_proc->key = pid;
  new_proc->value = proc;

  return new_proc;
}

/*
  this function creates an entry of the page table
  input: pointer to the page struct
  return: a pointer to the node_pt struct
*/
node_pt *create_entry_pt(page *ptr)
{
  node_pt *new_entry = malloc(sizeof(node_pt));
  handle_malloc_error(new_entry);

  char *key_str = (char *)malloc(sizeof(char) * (strlen(ptr->pid) +
                                                 strlen(ptr->vpn) + 2));
  handle_malloc_error(key_str);

  snprintf(key_str, strlen(ptr->pid) + strlen(ptr->vpn) + 2, "%s %s",
           ptr->pid, ptr->vpn);

  new_entry->key = key_str;
  new_entry->value = ptr;

  return new_entry;
}

/*
  compare the two keys of two nodes for the binary tree
  for the process table
  input: two pointers to process struct
  output: 1 if a > b
          0 if a == b
	  -1 if a < b

  note that: a constant parameter is declared in the case when it is 
  necessary that the value of the transferred object remains unchanged 
  in the body of the called function.
*/
int compare_proc(const void *a, const void *b)
{
  // cast the params into process*
  node_proc *tmp_a = (node_proc *)a;
  node_proc *tmp_b = (node_proc *)b;

  // compare the values
  if (strcmp(tmp_a->key, tmp_b->key) > 0)
  {
    return 1;
  }
  else if (strcmp(tmp_a->key, tmp_b->key) == 0)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

/*
  compare the two keys of two nodes for the binary tree
  for the page table
  input: two pointers to page struct
  output: 1 if a > b
          0 if a == b
	        -1 if a < b
*/
int compare_pt(const void *a, const void *b)
{
  // cast the params into node_pt*
  node_pt *tmp_a = (node_pt *)a;
  node_pt *tmp_b = (node_pt *)b;

  // compare the values
  if (strcmp(tmp_a->key, tmp_b->key) > 0)
  {
    return 1;
  }
  else if (strcmp(tmp_a->key, tmp_b->key) == 0)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}


/*
  this function adds an entry to the process table
  input: address of the pointer to the root of process table, 
         entry to be added
*/
void add_to_proc_table(void **root, node_proc *ptr)
{
  void *result;
  node_proc *exist;

  if ((result = tsearch(ptr, root, compare_proc)) == NULL)
  {
    // add to process table failed
    fprintf(stderr, "insufficient memory\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    // check if the to-be-added node's key already existed
    exist = *(node_proc **)result;

    if (exist != ptr)
    {
      // the ptr that we want to add already exists
      // no need to add to table
      free(ptr);
    }
  }
}

/*
  this function adds an entry to the page table
  input: address of the pointer to the root of page table 
         entry to be added
*/
void add_to_pt(void **root, node_pt *ptr)
{
  // printf("add to page table: <%s, %ld>\n", ptr->key, ptr->value->ppn);
  void *result;
  node_pt *exist;

  if ((result = tsearch(ptr, root, compare_pt)) == NULL)
  {
    // add to process table failed
    fprintf(stderr, "insufficient memory\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    // check if the to-be-added node's key already existed
    exist = *(node_pt **)result;

    if (exist != ptr)
    {
      // the ptr that we want to add already exists
      // no need to add to table
      free(ptr);
    }
  }
}


/*
  this function finds the node in the process table if there's one
  input: root of the process table, pid(char*)
*/
node_proc *find_proc(void **root, char *key)
{
  void *result;
  node_proc *node;
  node_proc search_node;

  search_node.key = key;

  if ((result = tfind(&search_node, root, compare_proc)) == NULL)
  {
    // no node found
    node = NULL;
  }
  else
  {
    // node found
    node = *(node_proc **)result;
  }

  return node;
}

/*
  this function finds the node in the page table if there's one
  input: root of the page table, string "pid vpn"
*/
node_pt *find_pt(void **root, char *key)
{
  void *result;
  node_pt *node;
  node_pt search_node;

  search_node.key = key;

  if ((result = tfind(&search_node, root, compare_pt)) == NULL)
  {
    // no node found
    node = NULL;
  }
  else
  {
    // node found
    node = *(node_pt **)result;
  }

  return node;
}

/*
  this function deletes a node in process table
  if not found, then do nothing
  input: root of process table, unsigned long pid
*/
void delete_proc(void **root, char *key)
{
  node_proc *node;

  if ((node = find_proc(root, key)) == NULL)
  {
    // nothing to delete
  }
  else
  {
    tdelete(node, root, compare_proc);
    free(node);
  }
}

/*
  this function deletes a node in page table
  if not found, then do nothing
  input: root of page table, string "pid vpn"
*/
void delete_pt(void **root, char *key)
{
  node_pt *node;

  if ((node = find_pt(root, key)) == NULL)
  {
    // nothing to delete
  }
  else
  {
    tdelete(node, root, compare_pt);
    free(node);
  }
}
