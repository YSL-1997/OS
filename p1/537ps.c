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
    struct dirent *myFile;
    DIR* myDirectory = opendir(path); // Upon successful completion, opendir() returns a pointer to an object of type DIR   
    if(myDirectory){ // read dir successfully, next, need to read the desired filename
        FILE* fptr = fopen(filename, "r");
        if(fptr == NULL){ // failed to open the desired file
            printf("Cannot open file: %s.\n", filename);
            exit(0);
        }
      	else{
            closedir(myDirectory);
            return fptr;
	}
    }
    return NULL;
}

int main(int argc, char *argv[]){
    int c = 0;
    if(argc == 1){ // basically no other options listed(i.e. 537ps is the only thing that on the cmd), just display information for all processes of the current user
        return 0;
    }
    while((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1){ // read the argument
		int s_flag = 0; // defaults to be false
		int U_flag = 1; // defaults to be true
		int S_flag = 0; // defaults to be false
		int v_flag = 0; // defaults to be false
		int c_flag = 1; // defaults to be true
		
        switch(c){
        case 'p':
            long int processID = atoi(optarg); // need to handle the PID provided by the user in optarg
            DIR* myDirectory;
            struct dirent* myFile;
            return 0;
             // if the user does not provide pid, then display information for all processes of the "current" user
            printf("optarg = %s\n", optarg);
            printf("optarg in int = %d\n", atoi(optarg));
            // TO DO
            
            
            
            // still need to manage the default for each case
            break;

        case 's': // Display the single-character state information about the process.In: stat file, third ("state") field. 
                    // Note that the information that you read from the stat file is a character string. 
                    // This option defaults to be false, so if it is not present, do not display this information. 
                    // -s- is valid but has no effect.
                
                //TODO
            if(optarg != "-" && optarg != NULL){
				printf("Value of errno: %d\n", errno);
				perror("argument after -s should be NULL or -");
			}
            if(optarg == "-" || optarg == NULL){
				char* path = "/proc/";
            	strncat(path, optarg, sizeof(optarg)-1); // path completed
            	char* filename = "stat"
            	FILE* fptr = readDirFile(path, filename)
            	int processID;
            	char filenameOfExecutable[1024];
            	char state;
            	fscanf(fptr, "%d %s %c", &processID, filenameOfExecutable, &state);
            	printf("state: %c. ", state);
            	fclose(fptr);
            	break;
			}
            
                
                
                
                
                                                                                                                            
        
        case 'U': // Display the amount of user time consumed by this process. In: stat file, "utime" field. 
                    // This option defaults to be true, so if it is not present, then this information is displayed. 
                    // -U- turns this option off.
            char* path = "/proc/";
            strncat(path, optarg, sizeof(optarg)-1); // path completed
            char* filename = "stat"
            FILE* fptr = readDirFile(path, filename);
            // Since in this case, utime locates at the 15th (14th if starts from 0) position of stat file, we try a different way from case 's'.
            // first need to use fgets() to get a string as input from the FILE* fptr
            char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
            char* s = (char*)malloc(sizeof(unsigned long) * 15); // stores all the reads
            const char delimeter[4] = " "; // delimeter is " " only
            while(fgets(line, sizeof(line), fptr)){
                char* tok = strtok(fptr, delimeter); // use strtok() to get the first token
                int i = 0;
                while(tok != NULL){
                    s[i] = tok;
                    i++;
                    tok = strtok(NULL, delimeter);
                }
            }
            // now we have s s.t. stores 15 tokens   
            printf("user time: %ul. ", s[13]); // utime is at the 14th location
            free(s);
            fclose(fptr);
            break;
            
        case 'S': // Display the amount of system time consumed so far by this process. In: stat file, "stime" field.
              // This option defaults to be false, so if it is not present, then this information is not displayed.
              // "-S-" is valid but has no effect.
            char* path = "/proc/";
            strncat(path, optarg, sizeof(optarg)-1); // path completed
            char* filename = "stat"
            FILE* fptr = readDirFile(path, filename)
            // Since in this case, utime locates at the 15th (14th if starts from 0) position of stat file, we try a different way from case 's'.
            // first need to use fgets() to get a string as input from the FILE* fptr
            char line[sizeof(unsigned long)*15]; // stat file has only one line, so need a buffer of enough size for fgets()
            char* s = (char*)malloc(sizeof(unsigned long) * 15); // stores all the reads
            const char delimeter[4] = " "; // delimeter is " " only
            while(fgets(line, sizeof(line), fptr)){
                char* tok = strtok(fptr, delimeter); // use strtok() to get the first token
                int i = 0;
                while(tok != NULL){
                    s[i] = tok;
                    i++;
                    tok = strtok(NULL, delimeter);
                }
            }
            // now we have s s.t. stores 15 tokens   
            printf("system time: %ul. ", s[14]); // stime is at the 15th location
            free(s);
            fclose(fptr);
            break;

        case 'v': // Display the amount of virtual memory currently being used (in pages) by this program. 
                  // In: statm file, first ("size") field. 
                  // This option defaults to be false, so if it is not present, then this information is not displayed. 
                  // -v- is valid but has no effect.
            char* path = "/proc/";
            strncat(path, optarg, sizeof(optarg)-1); // path completed
            char* filename = "statm"
            FILE* fptr = readDirFile(path, filename)
            long int size;
            fscanf(fptr, "%d", &size);
            printf("size: %d. ", size)
            fclose(fptr);
            break;

        case 'c': // Display the command-line that started this program. In cmdline file in process's directory. 
				  // Be careful on this one, because this file contains a list of null (zero byte) terminated strings. 
				  // This option defaults to be true, so if it is not present, then this information is displayed. 
				  // -c- turns this option off.
            // TO DO
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
    return 1;
}
