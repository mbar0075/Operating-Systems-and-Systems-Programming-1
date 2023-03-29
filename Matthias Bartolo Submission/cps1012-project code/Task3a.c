#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linenoise.h"

//Checking if file name is valid
int InvalidFileNameCheck(char* filename){
    //Array of invalid characters
    char invalidchars[]={'#','%','&','{','}','\\','<','>','*','?','/',' ','$','!','\'','\"',':','@','+','`','|','='};
    //Invalid file name if name has more than 31 characters
    if(strlen(filename)>31){
        printf("Filename cannot be larger than 31 characters \n");
        return 1;
    }
    //Checking if filename has any invalid characters 
    for(int i=0;i<strlen(filename);i++){
        for(int j=0;j<sizeof(invalidchars);j++){
            if(filename[i]==invalidchars[j]){
                printf("Filename cannot contain character %c\n",invalidchars[j]);
                return 1;
            }
        }
    }
    return 0;
}

//Function for checking if file exists
int FileExistCheck(char* filename){
    FILE* fp;
    //Opening file to check whether it exists
    if((fp=fopen(filename,"r"))==NULL){
        printf("File does not exist\n");
        return 1;
    }
    else{
        fclose(fp);
        return 0;
    }
}

int main(int argc, char **argv) {
    char *line;

    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    
    line = linenoise("");
        
        /* Do something with the string. */
        if (line[0] != '\0' ) {
            linenoiseHistoryAdd(line); /*Add to the history.*/
            linenoiseHistorySave("history.txt"); /*Save the history on disk. */ 
            
            char *token;

            char *args[124];

            /*get first token*/
            token = strtok(line," ");
            int counter=0;
            args[counter]=token;

            /*separates the input based on the spaces*/
            while(token != NULL){
                token = strtok(NULL," ");
                counter++;
                args[counter] = token;
            }

            for(int i=0;i<counter;i++){
                    //Checking for pipe operator
                    if(strncmp(args[i],"|",strlen(args[i]))==0){
                                //Error Checking
                                if(i==0 || i==(counter-1)){
                                    printf("| cannot be first or last element\n");
                                }
                                else{
                                    if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                        printf("| Syntax Error\n");
                                    }
                                }
                    //Checking for > operator
                    }else if(strncmp(args[i],">",strlen(args[i]))==0){
                            //Error Checking
                            if(i!=0 && i!=(counter-1)){
                                if(InvalidFileNameCheck(args[i+1])==1){
                                }
                                else{
                                    if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                        printf("> Syntax Error\n");
                                    }
                                }
                            }
                            else{
                                printf("> cannot be first or last element\n");

                            }
                    //Checking for >> operator
                    }else if(strncmp(args[i],">>",strlen(args[i]))==0){
                                //Error Checking
                                if(i!=0 && i!=(counter-1)){
                                    if(InvalidFileNameCheck(args[i+1])==1){
                                    }
                                    else{
                                        if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                            printf(">> Syntax Error\n");
                                        }
                                    }
                                }
                                else{
                                    printf(">> cannot be first or last element\n");
                                }
                    //Checking for < operator
                    }else if(strncmp(args[i],"<",strlen(args[i]))==0){
                                //Error Checking
                                if(i!=0 && i!=(counter-1)){
                                    if(InvalidFileNameCheck(args[i+1])==1 || FileExistCheck(args[i+1])==1){
                                    }
                                    else{
                                        if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                            printf("< Syntax Error\n");
                                        }
                                    }

                                }
                                else{
                                    printf("< cannot be first or last element\n");
                                }

                            }

            }
        
        } 
        free(line);

    return EXIT_SUCCESS;
}