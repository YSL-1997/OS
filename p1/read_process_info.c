/*
Authors:
Yusen Liu - NetID: liu797, CS login: yusen
Yingjia Gu - NetID: gu79 , CS login: yingjia
Project #1 - a simple version of the ps command
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include "read_process_info.h"


char s_value = 0;
// STATE info
char U_value[300] = "";
// USER TIME info
char S_value[300] = "";
// SYSTEM TIME info
int v_value = 0;
// VIRTUAL MEM info
char c_value[300] = "";
// CMDLINE info
//int p_option_list[100];
// stores all pids if there's more than one -p option
//int p_option_list_index = 0;
// stores the first available index in p_option_list


// concatenate the path(/proc) with pid and filename
char* path_cat(char* path, int pid, char* filename){
    char pid_str[20];
    sprintf(pid_str, "%d", pid);
    strcat(path, pid_str);
    strcat(path, filename);
    return path;
}

// open a file, returns the pointer to the file
FILE* open_file(char* path){
    FILE* fptr = fopen(path, "r");
    if(fptr == NULL){
    // printf("Cannot open file with path: %s\n", path);
    exit(1);
    }
    return fptr;
}

// case 's': state info
void s_info(int pid){
    char path[100] = "/proc/";
    char* filename = "/stat";
    char* read_path = path_cat(path, pid, filename); // get the path
    FILE* fptr = open_file(read_path); // open the file
    // next, create three vars to store info, only state is important
    char buffer[800];
    fgets(buffer, 800, fptr);
    int processID; // if error happens, then change long to int
    char filenameOfExecutable[1024];
    char state;
    sscanf(buffer, "%d %s %c", &processID, filenameOfExecutable, &state);// if error happens, then change %ld to %d
    if(processID == pid){ // check if the pid matches what we have read
        s_value = state; // return the state
    }
    else{
        fclose(fptr);
        perror("PID does not match what we have read\n");
        exit(1);
    }
    fclose(fptr);
}

// case 'U': user time info
void U_info(int pid){ // check condition: count=14
  char path[100] = "/proc/";
  char* filename = "/stat";
  char* read_path = path_cat(path, pid, filename);
  FILE* fptr = open_file(read_path);
  // Since in this case, utime locates at the 14th (13th if starts from 0) position of stat file, we try a different way from case 's'.
  // first need to use fgets() to get a string as input from the FILE* fptr
  char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
  const char delimeter[4] = " "; // delimeter is " "
  int count = 0;
  while(fgets(line, sizeof(line), fptr) != NULL){
    char* tok = strtok(line, delimeter); // use strtok() to get the first token
    while(tok != NULL){
      count += 1;
      if(count == 14){
        memset(U_value, 0, strlen(U_value));
        strcpy(U_value, tok);
        break;
      }
      tok = strtok(NULL, delimeter); // get the next token
    }
    break;
  }
  fclose(fptr);
}

// case 'S': system time info
void S_info(int pid){ // check condition: count=15
  char path[100] = "/proc/";
  char* filename = "/stat";
  char* read_path = path_cat(path, pid, filename);
  FILE* fptr = open_file(read_path);
  // in this case, stime locates at the 15th (14th if starts from 0) position of stat file
  // first need to use fgets() to get a string as input from the FILE* fptr
  char line[sizeof(unsigned long)*17]; // stat file has only one line, so need a buffer of enough size for fgets()
  const char delimeter[4] = " "; // delimeter is " "
  int count = 0;
  while(fgets(line, sizeof(line), fptr) != NULL){
    char* tok = strtok(line, delimeter); // use strtok() to get the first token
    while(tok != NULL){
      count += 1;
      if(count == 15){
        memset(S_value, 0, strlen(S_value));
        strcpy(S_value, tok);
        break;
      }
      tok = strtok(NULL, delimeter); // get the next token
    }
    break;
  }
  fclose(fptr);
}


// case 'v': virtual memory info
void v_info(int pid){
  char path[100] = "/proc/";
  char* filename = "/statm";
  char* read_path = path_cat(path, pid, filename);
  FILE* fptr = open_file(read_path);
  int size;
  fscanf(fptr, "%d", &size);
  v_value = size;
  fclose(fptr);
}


// case 'c': cmdline info
void c_info(int pid){
  char path[100] = "/proc/";
  char* filename = "/cmdline";
  char* read_path = path_cat(path, pid, filename);
  FILE* fptr = open_file(read_path);
  char c[200];
  if(fscanf(fptr, "%s", c) < 0){
    exit(1);
  }
  fclose(fptr);
  memset(c_value, 0, strlen(c_value));
  strncpy(c_value, c, strlen(c));
}

void produce_output(int pid, int s, int U, int S, int v, int c){
  if(pid == 0){
    return;
  }
  char path[100] = "/proc/";
  char* filename = "/stat";
  char* read_path = path_cat(path, pid, filename);
  FILE* fptr = open_file(read_path);
  if(fptr == NULL){
    return;
  }
  printf("PID: %d, ", pid);
  if(s == 1){
    s_info(pid);
    printf("STATE: %c, ", s_value);
  }
  if(U == 1){
    U_info(pid);
    printf("USER TIME: %s, ", U_value);
  }
  if(S == 1){
    S_info(pid);
    printf("SYS TIME: %s, ", S_value);
  }
  if(v == 1){
    v_info(pid);
    printf("VIRTUAL MEM: %d, ", v_value);
  }
  if(c == 1){
    c_info(pid);
    printf("CMDLINE: %s", c_value);
  }
  printf("\n");
}
