#ifndef THREADS_H_
#define THREADS_H_

#include "Queue.h"
#include <ctype.h>
#include <pthread.h>


void* func_reader(void* arg);
void* func_munch1(void* args);
void* func_munch2(void* args);
void* func_writer(void* arg);
Multi_args* CreateMultiArgs(void* q1, void* q2);


#endif
