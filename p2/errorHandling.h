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

// this function is called by the rest of the functions
void check_and_print(int err, char* function_name);

// on success, gettimeofday() returns 0;
// on error, -1 is returned
void handle_gettime_error(int err);

// on success, sem_init(), sem_wait(), sem_post() return 0;
// -1 is returned on error, errno is set to indicate the error
void handle_sem_init_error(int err);

void handle_sem_wait_error(int err);

void handle_sem_post_error(int err);

// on success, pthread_create() returns 0
void handle_pthread_create_error(int err);

// on success, pthread_join() returns 0
void handle_pthread_join_error(int err);

#endif
