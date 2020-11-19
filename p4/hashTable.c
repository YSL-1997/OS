/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #4 - page replacement simulator
*/

/*
  This module is for building the hash table
  In this project, we basically need 2 hash tables: 
  page table & inverted page table
*/


#include "hashTable.h"


/*
  This is a hash function for page table & inverted page table
  For hash table:
    input: (pid, VPN)
    return: entryNo of the pair (pid, VPN, PPN)
  For inverted hash table: 
    input: PPN
    return: entryNo of the pair (pid, VPN, PPN)
*/
unsigned long hash_func(unsigned long val, int table_size)
{
  return val % table_size;
}

/*
  This function is responsible for the realloc if alloc'd space is not enough
  input: a pointer to the alloc'd space
  return: a pointer to the alloc'd space with more space
*/
void* my_realloc(void* ptr, size_t size)
{
  ptr = realloc(ptr, size);
  handle_malloc_error(ptr);
  return ptr;
}


/*
  This function is used for creating hash table and inverted hash table
  Each entry of the created table stores a pointer to a page struct
  input: size of the table
  return: a list of pointers to page struct
*/

page** create_page_table(unsigned long size)
{
  page** page_table = malloc(sizeof(page*) * size);
  handle_malloc_error(page_table);
  return page_table;
}

/*
  This function inserts a pointer to a page struct to a page table
  input: pointer to the table, size of the table, pointer to a page struct
  return: NULL
*/
void insert_pt(page** table, unsigned long size, page* ptr)
{
  unsigned long val = ptr->pid + ptr->vpn;
  unsigned long index = hash_func(val, size);

  page* tmp = table[index];
  while(tmp != NULL){
    tmp = tmp->pt_next;
  }

  tmp->pt_next = ptr;
}


/*
  This function inserts a pointer to a page struct to inverted page table
  input: pointer to the table, size of the table, pointer to a page struct
  return: NULL
*/
void insert_ipt(page** table, unsigned long size, page* ptr)
{
  unsigned long index = hash_func(ptr->ppn, size);

  page* tmp = table[index];
  while(tmp != NULL){
    tmp = tmp->ipt_next;
  }

  tmp->ipt_next = ptr;
}



delete
