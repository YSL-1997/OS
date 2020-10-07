#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

void reader(Queue* q){
    int i = 0; // index of current lenght
    int j = 0;
    FILE *fp;
    int MAX_LENGTH = 4096;
    char *buffer = malloc(MAX_LENGTH * sizeof(char));
    char ch = 'a';
    if((fp = fopen("bigfile.txt", "r") ) == NULL){
        perror("Could not get info from stdin");
        exit(EXIT_FAILURE);
    }

    while(ch != EOF){
        ch = fgetc(fp);

        if(i >= MAX_LENGTH){
            buffer[i-1] = '\0';
            fprintf(stderr, "Input line too long.\n");
            j++;
            printf("line id: %d, %s \n", j, buffer);
            while(ch != '\n' && ch != EOF){
                ch = getc(fp) ;   
            }
            i = 0;
            //free(buffer);
            //buffer = malloc(MAX_LENGTH * sizeof(char));
            continue;
        }
        buffer[i] = (char)ch;
        i++;
        if( ch == '\n' || ch == EOF){
            buffer[i] = '\0'; // meaning that buffer[0~i] is valid
            // need to put into a new malloc'ed str.
            char* ret_str = malloc((i+1) * sizeof(char));
            strncpy(); // store what's in buffer to ret_str
            EnqueueString(q, ret_str);
            i = 0;
            if(ch == EOF)
                break;
        }
    }
    fclose(fp)
    free(buffer);
    return 0;
}


int munch1(){
    char *buffer = malloc(MAX_LENGTH * sizeof(char));
    // readerQ->DequeueString(readerQ, buffer);
    int i = 0;
    if(buffer != NULL){
        while(buffer[i] != '\0'){
            if(buffer[i] == ' ')
                buffer[i] = '*';
            i++; 
        }
        // readerQ->EnqueueString(munchQ, buffer);}
    }
    // how to deal with the buffer = NULL
}

int munch2(){

    char *buffer = malloc(MAX_LENGTH * sizeof(char));
    // readerQ->DequeueString(readerQ, buffer);
    int i = 0;
    if(buffer != NULL){
        while(buffer[i] != '\0'){
            if(buffer[i]  >=97 && buffer[i] <=122)
                buffer[i] -= 32;
            i++; 
        }
        // readerQ->EnqueueString(munchQ, buffer);}
    }
    // how to deal with the buffer = NULL
}

int write(){
    
}

    
    
