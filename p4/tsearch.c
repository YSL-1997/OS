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
node_proc *create_entry_proc(unsigned long pid, process *proc)
{
  node_proc *new_proc = malloc(sizeof(node_proc));
  handle_malloc_error(new_proc);

  new_proc->key = pid;
  new_proc->value = proc;

  return new_proc;
}

/*
  this function creates an entry of the page table
  input: string "pid vpn", pointer to the page struct
  return: a pointer to the node_pt struct
*/
node_pt *create_entry_pt(char *key, page *value)
{
  node_pt *new_entry = malloc(sizeof(node_pt));
  handle_malloc_error(new_entry);

  new_entry->key = key;
  new_entry->value = value;

  return new_entry;
}


/*
  this function creates an entry of the inverted page table
  input: unsigned long ppn, pointer to the page struct
  return: a pointer to the node_pt struct
*/
node_pt *create_entry_ipt(unsigned long key, page *value)
{
  node_ipt *new_entry = malloc(sizeof(node_ipt));
  handle_malloc_error(new_entry);

  new_entry->key = key;
  new_entry->value = value;

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
  process *tmp_a = (process *)a;
  process *tmp_b = (process *)b;

  // compare the values
  if (tmp_a->pid > tmp_b->pid)
  {
    return 1;
  }
  else if (tmp_a->pid == tmp_b->pid)
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
  // cast the params into process*
  page *tmp_a = (page *)a;
  page *tmp_b = (page *)b;

  // malloc the two keys to be compared
  char *str_a = (char *)malloc(sizeof(char) * (strlen(tmp_a->pid) +
                                               strlen(tmp_a->vpn) + 2));
  char *str_b = (char *)malloc(sizeof(char) * (strlen(tmp_b->pid) +
                                               strlen(tmp_b->vpn) + 2));
  strcat(str_a, tmp_a->pid);
  strcat(str_a, " ");
  strcat(str_a, tmp_a->vpn);

  strcat(str_b, tmp_b->pid);
  strcat(str_b, " ");
  strcat(str_b, tmp_b->vpn);

  // compare the values
  if (str_a > str_b)
  {
    return 1;
  }
  else if (str_a == str_b)
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
  for the inverted page table
  input: two pointers to page struct
  output: 1 if a > b
          0 if a == b
	        -1 if a < b
*/
int compare_ipt(const void *a, const void *b)
{
  // cast the params into process*
  page *tmp_a = (page *)a;
  page *tmp_b = (page *)b;

  if (tmp_a->ppn > tmp_b->ppn)
  {
    return 1;
  }
  else if (tmp_a->ppn == tmp_b->ppn)
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
  this function adds an entry to the inverted page table
  input: address of the pointer to the root of inverted pt 
         entry to be added
*/
void add_to_ipt(void **root, node_ipt *ptr)
{
  void *result;
  node_ipt *exist;

  if ((result = tsearch(ptr, root, compare_ipt)) == NULL)
  {
    // add to process table failed
    fprintf(stderr, "insufficient memory\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    // check if the to-be-added node's key already existed
    exist = *(node_ipt **)result;

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
  input: root of the process table, pid(unsigned long)
*/
node_proc* find_proc(void** root, unsigned long key)
{
  void* result;
  node_proc* node;
  node_proc search_node;

  search_node.key = key;

  if((result = tfind(&search_node, root, compare_proc)) == NULL){
    // no node found
    printf("no node found\n");
  }
  else{
    // node found
    node = *(node_proc**)result;
  }

  return node;
}

/*
  this function finds the node in the page table if there's one
  input: root of the page table, string "pid vpn"
*/
node_proc* find_pt(void** root, char* key)
{
  void* result;
  node_pt* node;
  node_pt search_node;

  search_node.key = key;

  if((result = tfind(&search_node, root, compare_pt)) == NULL){
    // no node found
    printf("no node found\n");
  }
  else{
    // node found
    node = *(node_pt**)result;
  }

  return node;
}

/*
  this function finds the node in the inverted page table if there's one
  input: root of the inverted page table, unsigned long ppn
*/
node_proc* find_ipt(void** root, unsigned long key)
{
  void* result;
  node_ipt* node;
  node_ipt search_node;

  search_node.key = key;

  if((result = tfind(&search_node, root, compare_ipt)) == NULL){
    // no node found
    printf("no node found\n");
  }
  else{
    // node found
    node = *(node_ipt**)result;
  }

  return node;
}

/*
  this function deletes a node in process table
  if not found, then do nothing
  input: root of process table, unsigned long pid
*/
void delete_proc(void** root, unsigned long key)
{
  node_proc* node;

  if((node = find_proc(root, key)) == NULL){
    // nothing to delete
  }
  else{
    tdelete(node, root, compare_proc);
    free(node);
  }
}

/*
  this function deletes a node in page table
  if not found, then do nothing
  input: root of page table, string "pid vpn"
*/
void delete_pt(void** root, char* key)
{
  node_pt* node;

  if((node = find_pt(root, key)) == NULL){
    // nothing to delete
  }
  else{
    tdelete(node, root, compare_pt);
    free(node);
  }
}

/*
  this function deletes a node in inverted table
  if not found, then do nothing
  input: root of inverted page table, unsigned long ppn
*/
void delete_ipt(void** root, unsigned long key)
{
  node_ipt* node;

  if((node = find_ipt(root, key)) == NULL){
    // nothing to delete
  }
  else{
    tdelete(node, root, compare_ipt);
    free(node);
  }
}

/*
  this function frees node when destroying the process table
*/
void free_proc(void* ptr)
{
  node_proc* node = ptr;
  free(node);
}

/*
  this function frees node when destroying the page table
*/
void free_pt(void* ptr)
{
  node_pt* node = ptr;
  free(node);
}

/*
  this function frees node when destroying the inverted page table
*/
void free_ipt(void* ptr)
{
  node_ipt* node = ptr;
  free(node);
}