#ifndef PAGE_H_
#define PAGE_H_

typedef struct page{
  unsigned long pid;
  // process id
  
  unsigned long vpn;
  // virtual page number
  
  unsigned long ppn;
  // physical page number
  
  int ref_bit;
  // reference bit
  
  int valid_bit;
  // valid bit

  struct page* pt_next;
  // pointer to the next page in page table
  
  struct page* pt_prev;
  // pointer to the previous page in page table
  
  struct page* ipt_next;
  // pointer to the next page in inverted page table
  
  struct page* ipt_prev;
  // pointer to the previous page in inverted page table

  struct page* free_next;
  // pointer to the next page in free list

  struct page* free_prev;
  // pointer to the previous page in free list
  
  struct page* ram_next;
  // pointer to the next page in RAM list

  struct page* ram_prev;
  // pointer to the previous page in RAM list

}page;


#endif