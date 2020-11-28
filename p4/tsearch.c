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
int compare_proc(const void* a, const void* b)
{
  // cast the params into process*
  process* tmp_a = (process*)a;
  process* tmp_b = (process*)b;

  // compare the values
  if(tmp_a->pid > tmp_b->pid){
    return 1;
  }
  else if(tmp_a->pid == tmp_b->pid){
    return 0;
  }
  else{
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
int cpr_proc(void* a, void* b)
{
  // cast the params into process*
  process* tmp_a = (process*)a;
  process* tmp_b = (process*)b;

  
  // compare the values
  if(tmp_a->pid > tmp_b->pid){
    return 1;
  }
  else if(tmp_a->pid == tmp_b->pid){
    return 0;
  }
  else{
    return -1;
  }
}


/*
  this function converts unsigned long to string
  input: unsigned long
  return: converted string
*/
char* ul_to_str(unsigned long x)
{
  
}