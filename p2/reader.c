#include <stdio.h>
#include <stdlib.h>


int Reader(){
    // int MAX_BUFFER = 50960;
    // char *buffer = malloc(MAX_BUFFER * sizeof(char));
    // char *p;
    // int c;

    // while(fgets(buffer, MAX_BUFFER, stdin) != NULL){
    //     if((p = strchr(buf, "\n")) == NULL){
    //         fprintf(stderr, "Input line too long.\n")
    //         while((c = fgetc(stdin)) != EOF && c != '\n'){}
    //     }
    //     *p = "\0"

    // }
    // if(feof(stdin)){
    //     return 0
    // }
    // else{
    //     perror("Could not get info from stdin")
    //     exit(EXIT_FAILURE)
    // }


    char *buf, *lbuf, *subbuf;
	size_t len;
    int MAX_LENGTH = 50960;

	lbuf = NULL;
	while ((buf = fgetln(stdin, &len))) {
        if (len <= MAX_LENGTH){
            if (buf[len - 1] == '\n')
                buf[len - 1] = '\0';
            else {
                /* EOF without EOL, copy and add the NUL */
                if ((lbuf = malloc(len + 1)) == NULL)
                    err(1, NULL);
                memcpy(lbuf, buf, len);
                lbuf[len] = '\0';
                buf = lbuf;
		    }   
        }

        else{
            if ((subbuf = malloc(MAX_LENGTH)) == NULL)
				    err(1, NULL);
			    memcpy(subbuf, buf, MAX_LENGTH-1);
                subbuf[MAX_LENGTH-1] = '\0'
                buf = subbuf;
                free(subbuf)
        }
	
	}
	free(lbuf);
    if(feof(stdin)){
        return 0
    }
    
	if (ferror(fp))
		err(1, "fgetln");

}
