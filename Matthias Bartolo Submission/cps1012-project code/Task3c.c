#include "Headerfile.h"


int main(int argc, char **argv) {
    char *line;

    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    
    while((line = linenoise("")) != NULL) {
        
        /* Do something with the string. */
        if (line[0] != '\0' ) {
            linenoiseHistoryAdd(line); /*Add to the history.*/
            linenoiseHistorySave("history.txt"); /*Save the history on disk. */ 
            
            char *token;

            char *args[SIZE];

            /*get first token*/
            token = strtok(line," ");
            //Adding token to args array
            int counter=0;
            args[counter]=token;

            //To find the no of pipelines
            int noOfPipes=1;

            /*seperates the input based on the spaces*/
            while(token != NULL){
                token = strtok(NULL," ");
                counter++;
                args[counter] = token;

                //Adding counter when pipe is found
                if(token != NULL && strncmp(token,"|",strlen(token))==0){
                    noOfPipes++;
                }
            }

            //variable used to check whether to execute external commands
            bool externalcommand =true;
            //Task 2
            //Checking for  builtin commands, if the user enters an external command
            //the program will not execute external commands through flag externalcommand
            for( int i=0; i<4;i++){//Looping through array of builtin commands
                    //Comparing argument 1 with the builtin commands and executes said command if they match
                    if(strcmp(args[0],builtin_list[i].name)==0){
                        builtin_list[i].method(args);
                        externalcommand=false;
                        break;
                    }
            }
            
            //Task 1
            if(externalcommand==true){
                //input file name and output file name variables and append flag
                char  *file_in=NULL;
                char  *file_out=NULL;
                bool append_out=false;
                //pipeline variabls
                char ** pipeline [noOfPipes];
                char * arguments[noOfPipes][SIZE];

                //Counters for pipeline
                int pcounter=0;
                int pipelinecounter=0;
                //Flag for errors
                bool error =false;
                //Looping throuhg all tokens
                for(int i=0;i<counter;i++){
                        //Checking for pipe operator
                        if(strncmp(args[i],"|",strlen(args[i]))==0){
                            //Error Checking
                            if(i==0 || i==(counter-1)){
                                printf("| cannot be first or last element\n");
                                error=true;
                                break;
                            }
                            else{
                                if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                    printf("| Syntax Error\n");
                                    error=true;
                                    break;
                                }
                                else{
                                    //Creating a new stage for pipeline
                                    pipeline[pipelinecounter]=arguments[pipelinecounter];
                                    pcounter=0;
                                    pipelinecounter++;
                                }
                            }

                        }
                        //Checking for > operator
                        else if(strncmp(args[i],">",strlen(args[i]))==0){
                            //Error Checking
                            if(i!=0 && i!=(counter-1)){
                                if(InvalidFileNameCheck(args[i+1])==1){
                                    error=true;
                                    break;
                                }
                                else{
                                    if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                        printf("> Syntax Error\n");
                                        error=true;
                                        break;
                                    }
                                    else{
                                        //setting output file to args[i+1] and incrementing count
                                        file_out=args[i+1];
                                        i++;
                                        }
                                }
                            }
                            else{
                                printf("> cannot be first or last element\n");
                                error=true;
                                break;
                            }
                        //Checking for >> operator
                        }else if(strncmp(args[i],">>",strlen(args[i]))==0){
                            //Error Checking
                            if(i!=0 && i!=(counter-1)){
                                if(InvalidFileNameCheck(args[i+1])==1){
                                    error=true;
                                    break;
                                }
                                else{
                                    if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                        printf(">> Syntax Error\n");
                                        error=true;
                                        break;
                                    }
                                    else{
                                        //setting output file to args[i+1], append flag to true and incrementing count
                                        append_out=true;
                                        file_out=args[i+1];
                                        i++;
                                    }
                                }
                            }
                            else{
                                printf(">> cannot be first or last element\n");
                                error=true;
                                break;
                            }
                        //Checking for < operator
                        }else if(strncmp(args[i],"<",strlen(args[i]))==0){
                            //Error Checking
                            if(i!=0 && i!=(counter-1)){
                                if(InvalidFileNameCheck(args[i+1])==1 || FileExistCheck(args[i+1])==1){
                                    error=true;
                                    break;
                                }
                                else{
                                    if((strncmp(args[i+1],"|",strlen(args[i+1]))==0)||(strncmp(args[i+1],">",strlen(args[i+1]))==0)||(strncmp(args[i+1],">>",strlen(args[i+1]))==0)||(strncmp(args[i+1],"<",strlen(args[i+1]))==0)){
                                        printf("< Syntax Error\n");
                                        error=true;
                                        break;
                                    }
                                    else{
                                        //setting input file to args[i+1] and incrementing count
                                        file_in=args[i+1];
                                        i++;
                                    }
                                }

                            }
                            else{
                                printf("< cannot be first or last element\n");
                                error=true;
                                break;
                            }

                        }//Adding token to current pipeline stage and incrementing pcounter
                        else{
                            arguments[pipelinecounter][pcounter]=args[i];
                            pcounter++;
                            arguments[pipelinecounter][pcounter]=NULL;
                        }
                }
                //If no error is found then program will execute the pipeline_pipeline_async function
                if(error==false){

                    pipeline[pipelinecounter]=arguments[pipelinecounter];
                    pipeline[pipelinecounter+1]=NULL;
                    int result = execute_pipeline_async( pipeline ,true,file_in,file_out,append_out,noOfPipes);
                }
            }
        } 
        free(line);
    }
    return EXIT_SUCCESS;
}