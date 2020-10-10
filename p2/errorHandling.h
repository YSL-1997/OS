#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_


#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// on success, malloc() returns a pointer to allocated memory;
// on error, NULL is returned
void handle_malloc_error(void* ptr);

// on success, sem_init() returns 0;
// -1 is returned on error, errno is set to indicate the error
void handle_sem_init_error(int err);

// on success, pthread_create() returns 0
void handle_pthread_create_error(int err);

// on success, pthread_join() returns 0
void handle_pthread_join_error(int err);

#endif
