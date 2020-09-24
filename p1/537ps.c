/*
Authors: 
	Yusen Liu - NetID: liu797, CS login: yusen
	Yingjia Gu - NetID: gu79 , CS login: yingjia
Project #1 - a simple version of the ps command
*/

/*
Notes:
  UNIX manual has many sections.
    Section 1: commands. E.g.ls, gcc, cat
    Section 2: UNIX system calls (calls directly to the UNIX kernel). e.g. fork, open, read (not used in this project)
    Section 3: UNIX library routines. E.g.atof(), strcpy()
Methods needed:
  getopt() - handles command-line argument parsing
    "man 3 getopt" and see the given example to get started
  readdir() - a library function that reads the contents of a Linux directory(/proc)
    "man 3 readdir"
Critical:
  Need to understand the format of the /proc file system (/procfs)
    "man 5 proc"
    See the sample program that opens a directory and reads the directory entries
    Most of the file in /proc are in text
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

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
int pid_list[10000]; 
// list to store all pids of current user
int pid_list_index = 0; 
// stores the first available index in pid_list
int p_option_list[100]; 
// stores all pids if there's more than one -p option
int p_option_list_index = 0; 
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


// ** produce a list of pid's by current user
// pseudo code:
// readdir() to read /proc
// foreach dir in /proc, read dir, check if status file exists, 
// 		if yes, then read status file, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
//			if the uid we read is equal to(use strcmp) uid, then (1)pid, (2)case 'U'(TIME), (3)case 'c'(cmd)
//			else continue
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

void output_by_p_option_list(int s, int U, int S, int v, int c){
	for (int i = 0; i < p_option_list_index; i++){
		int this_pid = p_option_list[i];
		produce_output(this_pid, s, U, S, v, c);
	}
}

int main(int argc, char *argv[]){
	//int p_occurs = 0;
	int s_flag = 0; // defaults to be false
	int U_flag = 1; // defaults to be true
	int S_flag = 0; // defaults to be false
	int v_flag = 0; // defaults to be false
	int c_flag = 1; // defaults to be true
	int c = 0;
	int pid = 0;
	//long v_value = 0;
	if(argc == 1){// 537ps is the only command
		pid_list_by_current_user();
		for(int i = 0; i < pid_list_index; i++){ 
			int cur_pid = pid_list[i];
			if(cur_pid == 0){
				continue;
			}
			produce_output(cur_pid, s_flag, U_flag, S_flag, v_flag, c_flag);
		}
	}
	
	while((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1){ // read the argument
		switch(c){
			case 'p':
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
