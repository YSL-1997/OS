#ifndef INPUT_H_
#define INPUT_H_

#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include "tsearch.h"
#include "errorHandling.h"

FILE *read_file(char *path);
char **parsing(char *string);
bool is_empty(char *s);
unsigned long option_parser(int argc, char *argv[]);
void *get_processes_info(process **runnable_head, process **runnable_tail,
                         unsigned long *num_process, char *filepath);


#endif