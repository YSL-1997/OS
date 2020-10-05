#include <stdio.h>
#include <stdlib.h>


int Reader(){
    int MAX_BUFFER = 50960;
    char *buffer = malloc(MAX_BUFFER * sizeof(char));
    char *p;
    int c;

    while(fgets(buffer, MAX_BUFFER, stdin) != NULL){
        if((p = strchr(buf, "\n")) == NULL){
            fprintf(stderr, "Input line too long.\n")
            while((c = fgetc(fp) != EOF && c != '\n')){}
        }
        *p = "\0"

    }
    if(feof(stdin)){
        return 0
    }
    else{
        perror("Could not get info from stdin")
        exit(EXIT_FAILURE)
    }
}