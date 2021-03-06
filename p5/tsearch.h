#ifndef TSEARCH_H_
#define TSEARCH_H_
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "page.h"
#include "process.h"
#include "errorHandling.h"
typedef struct node_proc
{
  char* key;
  process* value;
} node_proc;

// struct of page table
typedef struct node_pt
{
  char* key;
  page* value;
} node_pt;

// functions to create an entry
node_proc *create_entry_proc(char* pid, process *proc);
node_pt *create_entry_pt(page *ptr);

// functions used to compare
int compare_proc(const void *a, const void *b);
int compare_pt(const void *a, const void *b);

// functions that used to add the entry
void add_to_proc_table(void **root, node_proc *ptr);
void add_to_pt(void **root, node_pt *ptr);

// functions to find a node given the key
node_proc *find_proc(void **root, char* key);
node_pt *find_pt(void **root, char *key);

// functions to delete a node given the key
void delete_proc(void **root, char* key);
void delete_pt(void **root, char *key);


#endif