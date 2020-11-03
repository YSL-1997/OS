#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_


#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// functions on errorHandling
void handle_fgetc_error(FILE* ptr);
void handle_malloc_error(void* ptr);
void handle_stat_error(int err);
#endif