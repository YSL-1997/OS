#ifndef THREADS_H_
#define THREADS_H_

#include "Queue.h"
#include <ctype.h>
#include <pthread.h>


/*
  struct of the arguments
  aim to pass as argument to pthread_create()
*/
typedef struct Multi_args{
  Queue* arg1;
  Queue* arg2;
  
} Multi_args;


void* func_reader(void* arg);
void* func_munch1(void* args);
void* func_munch2(void* args);
void* func_writer(void* arg);

// the function that creates a Multi_args containing two args
Multi_args* CreateMultiArgs(void* q1, void* q2);


#endif
