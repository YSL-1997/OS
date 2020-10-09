#ifndef THREADS_H_
#define THREADS_H_

#include "Queue.h"
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

// struct of the arguments
typedef struct Multi_args{
  Queue* arg1;
  Queue* arg2;
} Multi_args;


void* func_reader(void* arg);
void* func_munch1(void* args);
void* func_munch2(void* args);
void* func_writer(void* arg);
Multi_args* CreateMultiArgs(Queue* q1, Queue* q2);

#endif
