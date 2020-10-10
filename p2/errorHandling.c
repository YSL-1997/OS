#include "errorHandling.h"


void handle_malloc_error(void* ptr)
{
  if(ptr == NULL){
    fprintf(stderr, "malloc() error: failed to malloc\n");
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}

void handle_sem_init_error(int err)
{
  if(err != 0){
    fprintf(stderr, "sem_init() error: %s\n", strerror(err));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}

// on success, pthread_create() returns 0
void handle_pthread_create_error(int err)
{
  if(err != 0){
    fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);   
  } 
}

// on success, pthread_join() returns 0
void handle_pthread_join_error(int err)
{
  if(err != 0){
    fprintf(stderr, "pthread_join() error: %s\n", strerror(err));
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
  }
}
