#ifndef __READ_PROCESS_INFO_H
#define __READ_PROCESS_INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

char* path_cat(char* path, int pid, char* filename);
FILE* open_file(char* path);
void s_info(int pid);
void U_info(int pid);
void S_info(int pid);
void v_info(int pid);
void c_info(int pid);
void produce_output(int pid, int s, int U, int S, int v, int c);

#endif
