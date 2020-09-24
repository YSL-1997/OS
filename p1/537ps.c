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

int pid_list[10000];
// list to store all pids of current user
int pid_list_index = 0;
int p_option_list[100];
// stores all pids if there's more than one -p option
int p_option_list_index = 0;
// stores the first available index in p_option_list


// ** produce a list of pid's by current user
// pseudo code:
// readdir() to read /proc
// foreach dir in /proc, read dir, check if status file exists,
// if yes, then read status file, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
//if the uid we read is equal to(use strcmp) uid, then (1)pid, (2)case 'U'(TIME), (3)case 'c'(cmd)
//else continue
void pid_list_by_current_user(){
  int uid = getuid();
  pid_list_index = 0; // pointer that points to the first available index in pid_list
  DIR* procDirectory = opendir("/proc"); // Upon successful completion, opendir() returns a pointer to an object of type DIR
  struct dirent *entry_in_proc; // Pointer for directory entry
  if(procDirectory == NULL){ // opendir returns NULL if couldn't open directory
    printf("Could not open directory /proc.\n" );
    exit(1);
  }
  // now, we read directory '/proc' successfully
  while ((entry_in_proc = readdir(procDirectory)) != NULL){
    if(!isdigit(entry_in_proc->d_name[0])){
      continue;
    }                                                                                                                       char pid_status_path[200] = "/proc/";
    strcat(pid_status_path, entry_in_proc->d_name);
    strcat(pid_status_path, "/status");
    FILE* fptr = fopen(pid_status_path, "r");
    if(fptr == NULL){ // meaning that status file is not in this directory
      printf("Cannot open file in path: %s.\n", pid_status_path);
      continue;
    }
    // now, we have successfully opened a status file in a <pid> directory
    // read the file line by line, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
    int bufferLength = 255;
    char buffer[bufferLength];
    int count = 0; // track the # of lines read
    while(fgets(buffer, bufferLength, fptr)) {
      // now, the buffer stores: (e.g. "Uid:    2628    2628    2628    2628")
      // extract r_uid from the buffer
      if(count == 8){
        char u[10];
        int r_uid;
        sscanf(buffer, "%s %d", u, &r_uid);
        if(r_uid == uid && r_uid != 0){// this process's info can be displayed
          //convert string entry_in_proc->d_name to int
          //store in the pid_list
          pid_list[pid_list_index] = atoi(entry_in_proc->d_name);
          pid_list_index += 1;
          break;
        }
        break;
      }
      count += 1;
    }
    fclose(fptr);
  }
  closedir(procDirectory);
  // now we have a list of pid's with info needs to be printed
}

// output if there's more than 1 -p options
void output_by_p_option_list(int s, int U, int S, int v, int c){
  for (int i = 0; i < p_option_list_index; i++){
    int this_pid = p_option_list[i];
    produce_output(this_pid, s, U, S, v, c);
  }
}
int main(int argc, char *argv[]){
  int p_flag = 0;
  int s_flag = 0; // defaults to be false
  int U_flag = 1; // defaults to be true
  int S_flag = 0; // defaults to be false
  int v_flag = 0; // defaults to be false
  int c_flag = 1; // defaults to be true
  int c = 0;
  int pid = 0;
  //long v_value = 0;
  //printf("%d",argc);


  while((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1){ // read the argument
    switch(c){
    case 'p':
      p_flag = 1;
      p_option_list[p_option_list_index] = atoi(optarg);
      p_option_list_index += 1;
      pid = atoi(optarg);
      break;

    case 's': // Display the single-character state information about the process.In: stat file, third ("state") field.
      // Note that the information that you read from the stat file is a character string.
      // This option defaults to be false, so if it is not present, do not display this information.
      // -s- is valid but has no effect.
      if(optarg != 0 && *optarg == '-'){ // -s- : turn off the s_flag
        s_flag = 0;
        break;
      }
      else { // -s : turn on the s_flag
        s_flag = 1;
      }
      break;

    case 'U': // Display the amount of user time consumed by this process. In: stat file, "utime" field.
      // This option defaults to be true, so if it is not present, then this information is displayed.
      // -U- turns this option off.
      if(optarg != 0 && *optarg == '-'){ // turn off flag
        U_flag = 0;
        break;
      }
      else { // turn on flag
        U_flag = 1;
        break;
      }

    case 'S': // Display the amount of system time consumed so far by this process. In: stat file, "stime" field.
      // This option defaults to be false, so if it is not present, then this information is not displayed.
      // "-S-" is valid but has no effect.
      if(optarg != 0 && *optarg == '-'){ // turn off flag
        S_flag = 0;
        break;
      }
      else { // turn on flag
        S_flag = 1;
        break;
      }

    case 'v': ; // we add ";" here to avoid the error: "a label can only be part of a statement and a declaration is not a statement"
      // Display the amount of virtual memory currently being used (in pages) by this program.
      // In: statm file, first ("size") field.
      // This option defaults to be false, so if it is not present, then this information is not displayed.
      // -v- is valid but has no effect.
      if(optarg != 0 && *optarg == '-'){ // turn off flag
        v_flag = 0;
        break;
      }
      else { // turn on flag
        v_flag = 1;
        break;
      }

    case 'c': // Display the command-line that started this program. In /proc/<pid>/cmdline
      // Be careful on this one, because this file contains a list of null (zero byte) terminated strings.
      // This option defaults to be true, so if it is not present, then this information is displayed.
      // -c- turns this option off.
      if(optarg != 0 && *optarg == '-'){ // turn off flag
        c_flag = 0;
        break;
      }
      else { // turn on flag
        c_flag = 1;
        break;
      }

    case '?': // getopt() returns "?" when getopt finds an option character in argv that was not included in options
      // OR, a missing option argument(PID in this case)
      if(optopt == 'p'){ // if after -p, argument is missed
        fprintf(stderr, "Option -%c needs argument(PID).\n", optopt);
      }
      else{
        fprintf(stderr, "Unknown option -- -%c.\n", optopt);
        exit(1);
      }

    default:
      fprintf(stderr, "getopt");
    }
  }
  if(argc == 1 || p_flag == 0){// 537ps is the only command
    pid_list_by_current_user();
    for(int i = 0; i < pid_list_index; i++){
      int cur_pid = pid_list[i];
      if(cur_pid == 0){
        continue;
      }
      produce_output(cur_pid, s_flag, U_flag, S_flag, v_flag, c_flag);
    }
  }
  if(argc > optind){ // the argument of the -p is invalid(has more than one arguments)
    // need to handle the case: 537ps -p 1 3
    perror("the argument of the -p is invalid(has more than one arguments)");
    exit(1);
  }
  else {
    if(p_option_list_index > 0){
      output_by_p_option_list(s_flag, U_flag, S_flag, v_flag, c_flag);
      return 0;
    }
    produce_output(pid, s_flag, U_flag, S_flag, v_flag, c_flag);
    return 0;
  }
  return 1;
}
