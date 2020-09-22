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



// readDir(char* path) returns true if successfully directed to the directory specified by path, false o.w.
// filename should start with "./"
// remember to call fclose(FILE*) to close the file, and closedir() to close the directory after producing the output
FILE* readDirFile(char* path, char* filename){
	DIR* myDirectory = opendir(path); // Upon successful completion, opendir() returns a pointer to an object of type DIR   
	if(myDirectory){ // read dir successfully, next, need to read the desired filename
		FILE* fptr = fopen(filename, "r");
		if(fptr == NULL){ // failed to open the desired file
			printf("Cannot open file: %s.\n", filename);
			exit(1);
		}
		else{
			closedir(myDirectory);
			return fptr;
		}
	}
	return NULL;
}


// ** produce the process info of all processes by current user
// int uid = getuid()
// readdir() to read /proc
// foreach dir in /proc, read dir, check if status file exists, 
// 		if yes, then read status file, Uid is on the 9th line, with 4 numbers listed, only need to get real uid, i.e. the first number
//			if the uid we read is equal to(use strcmp) uid, then (1)pid, (2)case 'U'(TIME), (3)case 'c'(cmd)
//			else continue

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





int main(int argc, char *argv[]){
    int c = 0;
	long int pid = 0;
	char pid_str[32]
    if(argc == 1){ // basically no other options listed(i.e. 537ps is the only thing that on the cmd), just display information for all processes of the current user
        return 0;
    }
    while((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1){ // read the argument
		// flags are used to handle duplicates
		int s_flag = 0; // defaults to be false
		int U_flag = 0; // defaults to be true
		int S_flag = 0; // defaults to be false
		int v_flag = 0; // defaults to be false
		int c_flag = 0; // defaults to be true
		
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
				
				itoa(pid, pid_str, 10); // convert pid from int to string, stored in pid_str
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
				if(s_flag != 0){
					break;
				}
				else {
					s_flag = 1;
				}
				if(strcmp(optarg, "-") != 0 && optarg != NULL){
					printf("Value of errno: %d\n", errno);
					perror("argument after -s should be NULL or -\n");
				}
				if(strcmp(optarg, "-") == 0 || optarg == NULL){
					char* path = "/proc/";
					strncat(path, pid_str, strlen(pid_str)); // path completed
					char* filename = "stat";
					FILE* fptr = readDirFile(path, filename);
					int processID;
					char filenameOfExecutable[1024];
					char state;
					fscanf(fptr, "%d %s %c", &processID, filenameOfExecutable, &state);
					if(processID == pid){ // check if the pid matches
						printf("state: %c. ", state);
						fclose(fptr);
						break;
					}
					else {
						fclose(fptr);
						perror("processID and pid not match\n");
						break;
					}
				}







			case 'U': // Display the amount of user time consumed by this process. In: stat file, "utime" field. 
						// This option defaults to be true, so if it is not present, then this information is displayed. 
						// -U- turns this option off.
				if(U_flag != 0){
					break;
				}
				else {
					U_flag = 1;
				}
				if(strcmp(optarg, "-") != 0 && optarg != NULL){
					printf("Value of errno: %d\n", errno);
					perror("argument after -U should be NULL or -");
				}
				if(strcmp(optarg, "-") == 0){ // Not display
					break;
				}
				if(optarg == NULL){
					char* path = "/proc/";
					strncat(path, pid_str, strlen(pid_str)); // path completed
					char* filename = "stat";
					FILE* fptr = readDirFile(path, filename);
					// Since in this case, utime locates at the 14th (13th if starts from 0) position of stat file, we try a different way from case 's'.
					// first need to use fgets() to get a string as input from the FILE* fptr
					char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
					char* s = (char*)malloc(sizeof(unsigned long) * 15); // stores all the reads
					const char delimeter[4] = " "; // delimeter is " " only
					while(fgets(line, sizeof(line), fptr)){
						char* tok = strtok(line, delimeter); // use strtok() to get the first token
						char* tmp = s;
						while(tok != NULL){
							strncpy(tmp, tok, strlen(tok));
							tmp += sizeof(unsigned long);
							tok = strtok(NULL, delimeter);
						}
					}
					// now we have s s.t. stores 15 tokens
					if(strcmp(s, pid_str) == 0){ // check if the pid matches
						printf("user time: %s. ", s+sizeof(unsigned long)*13); // utime is at the 14th location
						free(s);
						fclose(fptr);
						break;
					}
					else{
						free(s);
						fclose(fptr);
						perror("processID and pid not match\n");
						break;
					}

				}

			case 'S': // Display the amount of system time consumed so far by this process. In: stat file, "stime" field.
					  // This option defaults to be false, so if it is not present, then this information is not displayed.
					  // "-S-" is valid but has no effect.
				if(S_flag != 0){
					break;
				}
				else {
					S_flag = 1;
				}
				if(strcmp(optarg, "-") != 0 && optarg != NULL){
					printf("Value of errno: %d\n", errno);
					perror("argument after -S should be NULL or -");
				}
				if(strcmp(optarg, "-") == 0 || optarg == NULL){
					char* path = "/proc/";
					strncat(path, pid_str, strlen(pid_str)); // path completed
					char* filename = "stat";
					FILE* fptr = readDirFile(path, filename);
					// Since in this case, stime locates at the 15th (14th if starts from 0) position of stat file, we try a different way from case 's'.
					// first need to use fgets() to get a string as input from the FILE* fptr
					char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
					char* s = (char*)malloc(sizeof(unsigned long) * 15); // stores all the reads
					const char delimeter[4] = " "; // delimeter is " " only
					while(fgets(line, sizeof(line), fptr)){
						char* tok = strtok(line, delimeter); // use strtok() to get the first token
						char* tmp = s;
						while(tok != NULL){
							strncpy(tmp, tok, strlen(tok));
							tmp += sizeof(unsigned long);
							tok = strtok(NULL, delimeter);
						}
					}
					// now we have s s.t. stores 15 tokens
					if(strcmp(s, pid_str) == 0){ // check if the pid matches
						printf("system time: %s. ", s+sizeof(unsigned long)*14); // stime is at the 15th location
						free(s);
						fclose(fptr);
						break;
					}
					else{
						free(s);
						fclose(fptr);
						perror("processID and pid not match\n");
						break;
					}
				}

			case 'v': ; // we add ";" here to avoid the error: "a label can only be part of a statement and a declaration is not a statement"
						// Display the amount of virtual memory currently being used (in pages) by this program. 
						// In: statm file, first ("size") field. 
						// This option defaults to be false, so if it is not present, then this information is not displayed. 
						// -v- is valid but has no effect.
				if(v_flag != 0){
					break;
				}
				else {
					v_flag = 1;
				}
				if(strcmp(optarg, "-") != 0 && optarg != NULL){
					printf("Value of errno: %d\n", errno);
					perror("argument after -v should be NULL or -");
				}
				if(strcmp(optarg, "-") == 0 || optarg == NULL){
					char* path = "/proc/";
					strncat(path, pid_str, strlen(pid_str)); // path completed
					char* filename = "statm";
					FILE* fptr = readDirFile(path, filename);
					long int size;
					fscanf(fptr, "%ld", &size);
					printf("size: %ld. ", size);
					fclose(fptr);
					break;
				}

			case 'c': // Display the command-line that started this program. In cmdline file in process's directory. 
					  // Be careful on this one, because this file contains a list of null (zero byte) terminated strings. 
					  // This option defaults to be true, so if it is not present, then this information is displayed. 
					  // -c- turns this option off.
				if(c_flag != 0){
					break;
				}
				else {
					c_flag = 1;
				}
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
		perror("the argument of the -p is invalid(has more than one arguments)");
		exit(1);
	}
	else {
	// TO DO
	// produce output according to the flags.
	}
	return 1;
}
