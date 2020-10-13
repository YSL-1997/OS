#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_


#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// functions on errorHandling
void handle_malloc_error(void* ptr);
void check_and_print(int err, char* function_name);
void handle_gettime_error(int err);
void handle_sem_init_error(int err);
void handle_sem_wait_error(int err);
void handle_sem_post_error(int err);
void handle_pthread_create_error(int err);
void handle_pthread_join_error(int err);

#endif
