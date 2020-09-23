/*
Authors: Yusen Liu, Yingjia Gu
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

char s_value = 0;
char U_value[300] = "";
char S_value[300] = "";
long v_value = 0;
char c_value[300] = "";
long pid_list[10000]; // list to store all pids of current user
int pid_list_index = 0; // pointer that points to the first available index in pid_list
char buffer_for_long_to_str[20] = "";
//FILE* fptr = NULL;

// readDirFile(char* path, char* filename) returns a file pointer if successfully directed to the directory specified by path, false o.w.
// remember to call fclose(FILE*) to close the file, and closedir() to close the directory after producing the output
FILE* readDirFile(char* path, char* filename){
	DIR* myDirectory = opendir(path); // Upon successful completion, opendir() returns a pointer to an object of type DIR   
	if(myDirectory){ // read dir successfully, next, need to read the desired filename
		FILE* fptr = fopen(filename, "r");
		if(fptr == NULL){ // failed to open the desired file
			printf("Cannot open file: %s.\n", filename);
			closedir(myDirectory);
			exit(1);
		}
		else{
			closedir(myDirectory);
			return fptr;
		}
	}
	return NULL;
}


// this function traverses all files in /proc, returns 1 if pid valid; 0 if pid not in /proc
// be careful that pid_str is a char string
int hasPid(char* pid_str){
	DIR* myDirectory = opendir("/proc"); // Upon successful completion, opendir() returns a pointer to an object of type DIR
	struct dirent *entry; // Pointer for directory entry
	if(myDirectory == NULL)  // opendir returns NULL if couldn't open directory 
	{ 
		printf("Could not open directory /proc.\n" ); 
		return 0; 
	}
    while ((entry = readdir(myDirectory)) != NULL){
		if(strcmp(entry->d_name, pid_str) == 0){
			return 1;
		}
	}
    closedir(myDirectory);     
    return 0; // didn't find the entry whose name is pid_str
}

// convert str to int
long str_to_long(char* str){
	char* ptr;
	return strtol(str, &ptr, 10);
}

// convert long to string
char* long_to_str(long num){
	strcpy(buffer_for_long_to_str, "");
	sprintf(buffer_for_long_to_str, "%ld", num);
	return buffer_for_long_to_str;
}

// case 's': state info
void s_info(long pid){
	char path[100] = "/proc/";
	char* filename = "/stat";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid)));
	strncat(path, filename, strlen(filename));
	FILE* fptr = fopen(path, "r");
	if(fptr == NULL){ // failed to open the desired file
			printf("Cannot open file: %s.\n", filename);
			exit(1);
	}
	// next, create three vars to store info, only state is important
	long processID; // if error happens, then change long to int
	char filenameOfExecutable[1024];
	char state;
	fscanf(fptr, "%ld %s %c", &processID, filenameOfExecutable, &state);// if error happens, then change %ld to %d
	if(processID == pid){ // check if the pid matches what we have read
		fclose(fptr);
		s_value = state; // return the state
	}
	else{
		fclose(fptr);
		perror("PID does not match what we have read\n");
		exit(1);
	}
}

// case 'U': user time info
void U_info(long pid){
	char path[100] = "/proc/";
	char* filename = "/stat";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid)));
	strncat(path, filename, strlen(filename));
	FILE* fptr = fopen(path, "r");
	if(fptr == NULL){ // failed to open the desired file
			printf("Cannot open file: %s.\n", filename);
			exit(1);
	}
	// Since in this case, utime locates at the 14th (13th if starts from 0) position of stat file, we try a different way from case 's'.
	// first need to use fgets() to get a string as input from the FILE* fptr
	char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
	char* s = (char*)malloc(sizeof(unsigned long) * 15); // stores all the reads
	char* tmp = s;
	const char delimeter[4] = " "; // delimeter is " "
	while(fgets(line, sizeof(line), fptr) != NULL){
		char* tok = strtok(line, delimeter); // use strtok() to get the first token
		while(tok != NULL){
			strncpy(tmp, tok, strlen(tok)); // copy tok into tmp(which is s as well)
			tmp += sizeof(unsigned long); // modify tmp to point to the next block
			tok = strtok(NULL, delimeter); // get the next token
		}
		// now we have s s.t. stores 15 tokens
		if(strcmp(s, long_to_str(pid)) == 0){ // check if the pid matches
			free(s);
			fclose(fptr);
			strncpy(U_value, s+sizeof(unsigned long)*13, strlen(s+sizeof(unsigned long)*13)); // utime is at the 14th location
		}
		else{
			free(s);
			fclose(fptr);
			perror("PID does not match what we have read\n\n");
			exit(1);
		}
	}
}

// case 'S': system time info
void S_info(long pid){
	char path[100] = "/proc/";
	char* filename = "/stat";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid)));
	strncat(path, filename, strlen(filename));
	FILE* fptr = fopen(path, "r");
	if(fptr == NULL){ // failed to open the desired file
			printf("Cannot open file: %s.\n", filename);
			exit(1);
	}
	// in this case, stime locates at the 15th (14th if starts from 0) position of stat file
	// first need to use fgets() to get a string as input from the FILE* fptr
	char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
	char* s = (char*)malloc(sizeof(unsigned long) * 15); // stores all the reads
	char* tmp = s;
	const char delimeter[4] = " "; // delimeter is " " only
	while(fgets(line, sizeof(line), fptr) != NULL){
		char* tok = strtok(line, delimeter); // use strtok() to get the first token
		while(tok != NULL){
			strncpy(tmp, tok, strlen(tok)); // copy tok into tmp(which is s as well)
			tmp += sizeof(unsigned long); // modify tmp to point to the next block
			tok = strtok(NULL, delimeter); // get the next token
		}
		// now we have s s.t. stores 15 tokens
		if(strcmp(s, long_to_str(pid)) == 0){ // check if the pid matches
			free(s);
			fclose(fptr);
			strncpy(S_value, s+sizeof(unsigned long)*14, strlen(s+sizeof(unsigned long)*14)); // utime is at the 15th location
		}
		else{
			free(s);
			fclose(fptr);
			perror("PID does not match what we have read\n\n");
			exit(1);
		}
	}
}


// case 'v': virtual memory info
void v_info(long pid){
	char path[100] = "/proc/";
	char* filename = "/statm";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid)));
	strncat(path, filename, strlen(filename));
	FILE* fptr = fopen(path, "r");
	if(fptr == NULL){ // failed to open the desired file
			printf("Cannot open file: %s.\n", filename);
			exit(1);
	}
	long size;
	fscanf(fptr, "%ld", &size);
	fclose(fptr);
	v_value = size;
}


// case 'c': cmdline info
void c_info(long pid){
	char c[800];
	char path[100] = "/proc/";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid))); // path completed
	strcat(path,"/cmdline");
	FILE* fptr = fopen(path,"r");
	if(fptr == NULL){ // failed to open the desired file
		printf("Cannot open file: %s.\n", path);
		exit(1);
	}
	if(fscanf(fptr, "%s", c) < 0){
		exit(1);
	}
	fclose(fptr);
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
	long uid = getuid();
	pid_list_index = 0; // pointer that points to the first available index in pid_list
	DIR* procDirectory = opendir("/proc"); // Upon successful completion, opendir() returns a pointer to an object of type DIR
	struct dirent *entry_in_proc; // Pointer for directory entry
	if(procDirectory == NULL){ // opendir returns NULL if couldn't open directory 
		printf("Could not open directory /proc.\n" ); 
		exit(1); 
	}

	// now, we read directory '/proc' successfully
	while ((entry_in_proc = readdir(procDirectory)) != NULL){
		char pid_path[50] = "./";
		strncat(pid_path, entry_in_proc->d_name, strlen(entry_in_proc->d_name));
		DIR* pidDirectory = opendir(pid_path); // open pid directory
		if(pidDirectory == NULL){ // opendir returns NULL if couldn't open directory 
			printf("Could not open directory: %s.\n", pid_path); 
			continue; // read the next directory in /proc
		}

		// now, we open dir of one <pid> successfully (may not be pid dir)
		// read status file
		char* filename = "status";
		FILE* fptr = fopen(filename, "r");
		if(fptr == NULL){ // meaning that status file is not in this directory
			printf("Cannot open file: %s.\n", filename);
			closedir(pidDirectory);
			continue;
		}

		// now, we have successfully opened a status file in a <pid> directory
		// read the file line by line, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
		int bufferLength = 255;
		char buffer[bufferLength];
		int count = 0; // track the # of lines read
		while(fgets(buffer, bufferLength, fptr)) {
			if(count == 8){
				// now, the buffer stores: (e.g. "Uid:    2628    2628    2628    2628")
				// extract r_uid from the buffer
				char* token = strtok(buffer, " "); // extract the first token
				int count1 = 0; // track the # of tokens
				while(token != NULL){
					if(count1 == 1){
						// now, the r_uid of this process is stored in token
						long ret = str_to_long(token);
						if(ret == uid){// this process's info can be displayed
							//convert string entry_in_proc->d_name to int
							//store in the pid_list
							pid_list[pid_list_index] = str_to_long(entry_in_proc->d_name);
							pid_list_index += 1;
							break;
						}
						// if we go here, meaning that this process does not belong to current user
						break;
					}
					token = strtok(NULL, " ");
					count1 += 1;
				}
				break;
			}
			count += 1;
		}
		fclose(fptr);
		closedir(pidDirectory);
	}
	closedir(procDirectory);
	// now we have a list of pid's with info needs to be printed
}		


void produce_output(long pid, int s, int U, int S, int v, int c){
	printf("PID: %ld, ", pid);
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
		printf("VIRTUAL MEM: %ld, ", v_value);
	}
	if(c == 1){
		c_info(pid);
		printf("CMDLINE: %s", c_value);
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
	long pid = 0;
	//long v_value = 0;
	if(argc == 1){// 537ps is the only command
		pid_list_by_current_user();
		for(int i = 0; i < sizeof(pid_list)/sizeof(long); i++){ 
			long cur_pid = pid_list[i];
			produce_output(cur_pid, s_flag, U_flag, S_flag, v_flag, c_flag);
			printf("\n");
		}
	}
	
	while((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1){ // read the argument
		switch(c){
			case 'p':
				//p_occurs = 1;
				pid = str_to_long(optarg);
				break;

			case 's': // Display the single-character state information about the process.In: stat file, third ("state") field. 
						// Note that the information that you read from the stat file is a character string. 
						// This option defaults to be false, so if it is not present, do not display this information. 
						// -s- is valid but has no effect.
				if(strcmp(optarg, "-") != 0 && optarg != NULL){ // if this condition has an error, then try replacing NULL with ""
					printf("Value of errno: %d\n", errno);
					perror("argument after -s should be NULL or -\n");
					exit(1);
				}
				if(strcmp(optarg, "-") == 0){ // -s- : turn off the s_flag
					s_flag = 0;
					break;
				}
				if(optarg == NULL){ // -s : turn on the s_flag
					s_flag = 1;
				}
				break;

			case 'U': // Display the amount of user time consumed by this process. In: stat file, "utime" field. 
						// This option defaults to be true, so if it is not present, then this information is displayed. 
						// -U- turns this option off.
				if(strcmp(optarg, "-") != 0 && optarg != NULL){ // if this condition has an error, then try replacing NULL with ""
					printf("Value of errno: %d\n", errno);
					perror("argument after -U should be NULL or -");
					exit(1);
				}
				if(strcmp(optarg, "-") == 0){ // turn off flag
					U_flag = 0;
					break;
				}
				if(optarg == NULL){ // turn on flag
					U_flag = 1;
					break;
				}

			case 'S': // Display the amount of system time consumed so far by this process. In: stat file, "stime" field.
					  // This option defaults to be false, so if it is not present, then this information is not displayed.
					  // "-S-" is valid but has no effect.
				if(strcmp(optarg, "-") != 0 && optarg != NULL){ // if this condition has an error, then try replacing NULL with ""
					printf("Value of errno: %d\n", errno);
					perror("argument after -S should be NULL or -");
					exit(1);
				}
				if(strcmp(optarg, "-") == 0){ // turn off flag
					S_flag = 0;
					break;
				}
				if(optarg == NULL){ // turn on flag
					S_flag = 1;
					break;
				}

			case 'v': ; // we add ";" here to avoid the error: "a label can only be part of a statement and a declaration is not a statement"
						// Display the amount of virtual memory currently being used (in pages) by this program. 
						// In: statm file, first ("size") field. 
						// This option defaults to be false, so if it is not present, then this information is not displayed. 
						// -v- is valid but has no effect.
				
				if(strcmp(optarg, "-") != 0 && optarg != NULL){
					printf("Value of errno: %d\n", errno);
					perror("argument after -v should be NULL or -");
					exit(1);
				}
				if(strcmp(optarg, "-") == 0){ // turn off flag
					v_flag = 0;
					break;
				}
				if(optarg == NULL){ // turn on flag
					v_flag = 1;
					break;
				}

			case 'c': // Display the command-line that started this program. In /proc/<pid>/cmdline 
					  // Be careful on this one, because this file contains a list of null (zero byte) terminated strings. 
					  // This option defaults to be true, so if it is not present, then this information is displayed. 
					  // -c- turns this option off.
				if(strcmp(optarg, "-") != 0 && optarg != NULL){
					printf("Value of errno: %d\n", errno);
					perror("argument after -c should be NULL or -");
					exit(1);
				}
				if(strcmp(optarg, "-") == 0){ // turn off flag
					c_flag = 0;
					break;
				}
				if(optarg == NULL){ // turn on flag
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
		produce_output(pid, s_flag, U_flag, S_flag, v_flag, c_flag);
		return 0;
	}
	return 1;
}
