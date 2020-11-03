/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #3 - 537make
*/


#include "errorHandling.h"

// use ferror() to handle the error
void handle_fgetc_error(FILE* ptr)
{
  if(ferror(ptr)){
    fprintf(stderr, "fgetc error: %s\n", strerror(errno));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);   
  }
}


// on success, malloc returns a pointer to allocated memory
void handle_malloc_error(void* ptr)
{
  if(ptr == NULL){
    fprintf(stderr, "malloc() error: failed to malloc\n");
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);   
  }
}

// on success, stat returns 0; on error, -1 is returned
void handle_stat_error(int err)
{
  if(err !=0){
    fprintf(stderr, "stat error: %s\n", strerror(errno));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}