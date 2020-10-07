#ifndef READER_H_
#define READER_H_

#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <thread.h>
#include "Queue.h"
#include <pthread.h>

void func_reader(void* arg);

#endif
