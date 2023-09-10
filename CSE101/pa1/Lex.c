//Moore Macauley
//mmacule
//pa1

#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 300

int main(int argc, char const *argv[])
{
    FILE *in, *out;
    char line[MAX_LENGTH];

    if(argc != 3){
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
        if( in==NULL ){
        fprintf(stderr,"Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        fprintf(stderr,"Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    int currentChar = 0;
    int lineCount = 0;

    while((currentChar = getc(in)) != EOF){
        if(currentChar == '\n'){
            lineCount++;
        }
    }

    fclose(in);
    in = fopen(argv[1], "r");

    List list = newList();
    int value = 0;
    moveFront(list);

    char** fileArr = calloc(sizeof(char*), lineCount);
    for(int i = 0; i < lineCount; i++){
        fgets(line, MAX_LENGTH, in);
        char *temp = calloc(sizeof(char), strlen(line)+1);
        strcpy(temp, line);
        if(strchr(temp, '\n') != NULL && temp[0] != '\n'){
            int x = strlen(temp);
            temp[x-1] = '\0';
        }
        fileArr[i] = temp;
        moveFront(list);
        while(index(list) >= 0){
            value = get(list);
            if(strcmp(fileArr[i], fileArr[value]) <= 0){
                insertBefore(list, i);
                break;
            }
            moveNext(list);
            
        }
        if(index(list) == -1){
            append(list, i);
        }
    }
    
    moveFront(list);
    while(index(list) >= 0){
        if(fileArr[get(list)][0] != '\n'){
            fprintf(out, "%s", fileArr[get(list)]);
        }
        fprintf(out, "\n");
        free(fileArr[get(list)]);
        moveNext(list);
    }

    freeList(&list);
    free(fileArr);
    fclose(in);
    fclose(out);

    return 0;
}
