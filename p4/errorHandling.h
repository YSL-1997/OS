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
void check_and_print(int err, char* function_name);
// void handle_ftell_error(FILE* fp);

#endif
