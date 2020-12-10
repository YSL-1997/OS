/*
  Authors:
  Yusen Liu - NetID: liu797, CS login: yusen
  Yuting Yan - NetID: yan86 , CS login: yutingy
  Project #2 - Shared Memory Producer/Consumer Program
*/

#include "errorHandling.h"

// use ferror() to handle the error
void handle_fgetc_error(FILE *ptr)
{
  if (ferror(ptr))
  {
    fprintf(stderr, "fgetc error: %s\n", strerror(errno));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}

// on success, malloc returns a pointer to allocated memory
void handle_malloc_error(void *ptr)
{
  if (ptr == NULL)
  {
    fprintf(stderr, "malloc() error: failed to malloc\n");
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}

// this function will be called by the following functions
void check_and_print(int err, char *function_name)
{
  if (err != 0)
  {
    fprintf(stderr, "%s error: %s\n", function_name, strerror(err));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}

/* 
  on success, ftell() and ftello() shall return the current value of 
  the file-position indicator for the stream measured in bytes from the 
  beginning of the file.
   
  Otherwise, ftell() and ftello() shall return −1, and set errno to indicate 
  the error.
*/
long handle_ftell_error(long err)
{
  if (err == -1)
  {
    fprintf(stderr, "ftell error:%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  else
  {
    return err;
  }
}