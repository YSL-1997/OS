#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

int main(){
    int i = 0;
    int j = 0;
    FILE *fp;
    int MAX_LENGTH = 1096;
    char *buffer = malloc(MAX_LENGTH * sizeof(char));
    char ch;

    if((fp = fopen("bigfile.txt", "r") ) == NULL){
        perror("Could not get info from stdin");
        exit(EXIT_FAILURE);
    }

    while( ch != EOF){
        
        ch = getc(fp);

        if(i >= MAX_LENGTH){
            buffer[i -1] = '\0';
            fprintf(stderr, "Input line too long.\n");
            j++;
            printf("line id: %d, %s \n", j, buffer);
            while(ch != '\n' && ch != EOF){
                ch = getc(fp) ;   
            }
            i = 0;
            free(buffer);
            buffer = malloc(MAX_LENGTH * sizeof(char));
            continue;
        }
        buffer[i] = (char)ch;
        i++;
        if( ch == '\n' || ch == EOF){
            buffer[i] = '\0';
            // printf("%s", buffer);
            // sem
            // readerQ->EnqueueString(readerQ, buffer);
            i = 0;
            free(buffer);
            buffer = malloc(MAX_LENGTH * sizeof(char));
            j++;
            if(ch == EOF)
                break;
        }


    }

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

    
    