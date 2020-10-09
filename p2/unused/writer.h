#ifndef WRITER_H_
#define WRITER_H_

#include "Queue.h"
#include <pthread.h>

void* func_writer(void* q);

#endif
