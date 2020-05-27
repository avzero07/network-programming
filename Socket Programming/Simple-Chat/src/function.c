#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int transcribe(char* msg, char* filename){
    FILE *f =  fopen(filename,"a+");

    if(f==NULL){
        fprintf(stderr,"ERROR: Could not Open File!\n");
        return EXIT_FAILURE;
    }
    fprintf(f,"%s",msg);
    int p = fclose(f);

    return 0;
}
