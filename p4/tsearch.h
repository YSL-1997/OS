#ifndef TSEARCH_H_
#define TSEARCH_H_
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include "input.c"
#include "page.h"
#include "process.h"

typedef struct proc_node
{
  char* key;
  process* value;
} node_proc;

// struct of page table and inverted page table
typedef struct node_pt
{
  char* key;
  page* value;
} node_pt;




int compare_proc(const void *a, const void *b);
int compare_pt(const void *a, const void *b);
int compare_ipt(const void *a, const void *b);

#endif