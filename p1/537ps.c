/*
Authors: Yusen Liu, Yingjia Gu
Project #1 - a simple version of the ps command
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>


#define BUFFER_SIZE (512)

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
	long ret;
	return strtol(str, &ptr, 10);
}

// convert long to string
char* long_to_str(long num){
	char* buffer;
	sprintf(buffer, "%ld", num);
	return buffer;
}

// case 's': state info
char s_info(long pid){
	char* path = "/proc/";
	char* filename = "stat";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid))); // path completed
	FILE* fptr = readDirFile(path, filename);
	// next, create three vars to store info, only state is important
	long processID; // if error happens, then change long to int
	char filenameOfExecutable[1024];
	char state;
	fscanf(fptr, "%ld %s %c", &processID, filenameOfExecutable, &state);// if error happens, then change %ld to %d
	if(processID == pid){ // check if the pid matches what we have read
		fclose(fptr);
		return state; // return the state
	}
	else{
		fclose(fptr);
		perror("PID does not match what we have read\n");
		exit(1);
	}
}

// case 'U': user time info
char* U_info(long pid){
	char* path = "/proc/";
	char* filename = "stat";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid))); // path completed
	FILE* fptr = readDirFile(path, filename);
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
			return s+sizeof(unsigned long)*13; // utime is at the 14th location
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
char* S_info(long pid){
	char* path = "/proc/";
	char* filename = "stat";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid))); // path completed
	FILE* fptr = readDirFile(path, filename);
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
			return s+sizeof(unsigned long)*14; // utime is at the 15th location
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
long v_info(long pid){
	char* path = "/proc/";
	char* filename = "statm";
	strncat(path, long_to_str(pid), strlen(long_to_str(pid))); // path completed
	FILE* fptr = readDirFile(path, filename);
	long size;
	fscanf(fptr, "%ld", &size);
	fclose(fptr);
	return size;
}


int main(int argc, char *argv[]){
	int s_flag = 0; // defaults to be false
	int U_flag = 1; // defaults to be true
	int S_flag = 0; // defaults to be false
	int v_flag = 0; // defaults to be false
	int c_flag = 1; // defaults to be true
	int c = 0;
	long pid = 0;
	char pid_str[32]
	if(argc == 1){ // basically no other options listed(i.e. 537ps is the only thing that on the cmd), 
				   // just display information for all processes of the current user
		// ** produce the process info of all processes by current user
		// int uid = getuid()
		// readdir() to read /proc
		// foreach dir in /proc, read dir, check if status file exists, 
		// 		if yes, then read status file, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
		//			if the uid we read is equal to(use strcmp) uid, then (1)pid, (2)case 'U'(TIME), (3)case 'c'(cmd)
		//			else continue
		
		long uid = getuid();
		long pid_list[10000]; // list to store all pids of current user
		int pid_list_index = 0; // pointer that points to the first available index in pid_list
		DIR* procDirectory = opendir("/proc"); // Upon successful completion, opendir() returns a pointer to an object of type DIR
		struct dirent *entry_in_proc; // Pointer for directory entry
		if(procDirectory == NULL)  // opendir returns NULL if couldn't open directory 
		{ 
			printf("Could not open directory /proc.\n" ); 
			return 0; 
		}
		// read dir /proc successfully
		while ((entry_in_proc = readdir(procDirectory)) != NULL){
			char pid_path[50] = "./";
			strncat(pid_path, entry_in_proc->d_name, strlen(entry_in_proc->d_name));
			DIR* pidDirectory = opendir(pid_path);
			if(pidDirectory == NULL)  // opendir returns NULL if couldn't open directory 
			{ 
				printf("Could not open directory: %s.\n", pid_path); 
				return 0; 
			}
			// open dir of pid successfully
			// read status file
			char* filename = "status";
			FILE* fptr = fopen(filename, "r");
			if(fptr == NULL){ // failed to open the desired file
				printf("Cannot open file: %s.\n", filename);
				closedir(pidDirectory);
				exit(1);
			}
			// now we have fptr that points to status file
			// read the file line by line, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
			int bufferLength = 255;
			char buffer[bufferLength];
			int count = 0; // track the # of lines read
			while(fgets(buffer, bufferLength, fptr)) {
    			if(count == 8){
					// now the buffer stores: (e.g. "Uid:    2628    2628    2628    2628")
					// extract r_uid from the buffer
   					char* token = strtok(buffer, " "); // extract the first token
					int count1 = 0;
					while(token != NULL){
						if(count1 == 1){
							// now, uid of this process is stored in token
   							long ret = str_to_long(token);
							if(ret == uid){// this process's info can be displayed
										   //convert string entry_in_proc->d_name to int
										   //store in the pid_list
								pid_list[pid_list_index] = str_to_long(entry_in_proc->d_name);
								pid_list_index += 1;
							}
							else{
								break;
							}
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
		//return 0;
	}
	
	while((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1){ // read the argument
        switch(c){
			case 'p':
				// TO DO
				// need to handle the case: 537ps -p 1 3
				char pid_str[32];
				strncpy(pid_str, argv[optind-1], strlen(argv[optind-1]));
				
				
				if(hasPid(optarg) == 0){ // pid from user input is incorrect
					perror("error: process ID out of range");
					exit(1);
				}
				
				pid = atoi(optarg); // need to handle the PID provided by the user in optarg
				if(pid == 0)
				char pid_buffer[40]; // pid_buffer is a char array that stores
				
				sprintf(pid_str, "%ld", pid);// convert pid from int to string, stored in pid_str
				//itoa(pid, pid_str, 10); 
				int ruid = getuid(); // ruid is an int
				DIR* myDirectory = opendir("/proc");// Upon successful completion, opendir() returns a pointer to an object of type DIR
				if(myDirectory){ // read dir successfully, next, need to read the desired filename
	
				}
			
				return 0;
				 // if the user does not provide pid, then display information for all processes of the "current" user
				
				// TO DO



				// still need to manage the default for each case
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

			case 'c': // Display the command-line that started this program. In cmdline file in process's directory. 
					  // Be careful on this one, because this file contains a list of null (zero byte) terminated strings. 
					  // This option defaults to be true, so if it is not present, then this information is displayed. 
					  // -c- turns this option off.
				
				use fread() function
				// if -p pid, then print the cmd for pid
				// else, print cmd for all process by current user
				// cmd info is stored in /proc/<pid>/cmdline
				
				

				break;
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
	// TO DO
	// produce output according to the flags.
	}
	return 1;
}
