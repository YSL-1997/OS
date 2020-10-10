#ifndef THREADS_H_
#define THREADS_H_

#include "Queue.h"
#include <ctype.h>
#include <pthread.h>


// struct of the arguments
// aim to pass as argument to pthread_create()
typedef struct Multi_args{
  Queue* arg1;
  Queue* arg2;
} Multi_args;

// reader thread
void* func_reader(void* arg);

// munch1 thread
void* func_munch1(void* args);

// munch2 thread
void* func_munch2(void* args);

// writer thread
void* func_writer(void* arg);

// pass as argument to pthread_create()
Multi_args* CreateMultiArgs(void* q1, void* q2);


#endif
