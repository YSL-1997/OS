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
            return fptr;                                                                                                          }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    int c = 0;
    if(argc == 1){ // basically no other options listed(i.e. 537ps is the only thing that on the cmd), just display infor\
mation for all processes of the current user
    return 0;

  }
    while((c = getopt(argc, argv, "p:sUSvc")) != -1){ // read the argument
        switch(c){
        case 'p':
            long int processID = atoi(optarg); // need to handle the PID provided by the user in optarg
            DIR* myDirectory;
            struct dirent* myFile;
            return 0;
             // if the user does not provide pid, then display information for all processes of the "current" user
            printf("optarg = %s\n", optarg);
            printf("optarg in int = %d\n", atoi(optarg));

            break;
        case 's':                                                                                                           
            char* path = "/proc/";
            strncat(path, optarg, strlen(optarg)); // path completed
            char* filename = "stat"
            FILE* fptr = readDirFile(path, filename)
            int unused;
            char comm[1000];
            char state;
            int ppid;
            fscanf(fptr, "%d %s %c %d", &unused, comm, &state, &ppid);
            printf("state = %c\n", state);
            fclose(fptr);
            break;                                                                                                                
        
          case 'U': // Display the amount of user time consumed by this process. In: stat file, "utime" field. 
                    // This option defaults to be true, so if it is not present, then this information is displayed. 
                    // -U- turns this option off.
            char* path = "/proc/";
            strncat(path, optarg, strlen(optarg)); // path completed
            char* filename = "stat"
            FILE* fptr = readDirFile(path, filename)
            // TO DO  
              
              
            
            break;
            
        case 'S': // Display the amount of system time consumed so far by this process. In: stat file, "stime" field.
              // This option defaults to be false, so if it is not present, then this information is not displayed.
              // "-S-" is valid but has no effect.
            char* path = "/proc/";
            strncat(path, optarg, strlen(optarg)); // path completed
            char* filename = "stat"
            FILE* fptr = readDirFile(path, filename)
            

            break;

        case 'v': // Display the amount of virtual memory currently being used (in pages) by this program. 
                  // In: statm file, first ("size") field. 
                  // This option defaults to be false, so if it is not present, then this information is not displayed. 
                  // -v- is valid but has no effect.
            char* path = "/proc/";
            strncat(path, optarg, strlen(optarg)); // path completed
            char* filename = "statm"
            FILE* fptr = readDirFile(path, filename)
            long int size;
            fscanf(fptr, "%d", &size);
            printf("size = %d\n", size)
            fclose(fptr);
            break;

        case 'c':

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
