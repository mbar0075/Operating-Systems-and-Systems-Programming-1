#include "Headerfile.h"

int main(int argc, char **argv) {
    char *line;

    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    
    while((line = linenoise("tish$> ")) != NULL) {
        
        /* Do something with the string. */
        if (line[0] != '\0' ) {
            linenoiseHistoryAdd(line); /*Add to the history.*/
            linenoiseHistorySave("history.txt"); /*Save the history on disk. */ 
            

            //If there are an odd number of quotes, an error is shown
            int quotescount=0;
            int count=0; 
           
            for(count=0;line[count];count++){
               if(line[count]=='\"'&&line[count-1]!='\\')
                    quotescount++; 
            }
            if(quotescount%2!=0){
                printf("Syntax Error, number of quotes is invalid\n");
                continue;
            }

                char *args[SIZE];

                int counter=0;
                //To find the no of pipelines
                int noOfPipes=1;

                bool quotesflag=false;
                //Buffer used to hold the tokens 
                char buffer[SIZE][SIZE]={0};
                //buffer count to hold the index of buffer
                int buffercounter=0;
                //charcounter to hold the index of the character in the buffer
                int charcounter=0;

                //Parsing through line
                //Removing spaces before
                int ccount=0;
                while(line[ccount]==' '){
                    ccount++;
                }
                //Looping through all the characters in the line
                for(int i=ccount;i<strlen(line)+1;i++){
                    //Removing duplicate spaces
                    while(line[i+1]==' '&&line[i]==' '&&quotesflag==false){
                        i++;
                    }
                    //Removes space at the end of line
                    while(line[i+1]=='\0'&&line[i]==' '&&quotesflag==false){
                        i++;
                    }

                    //Checking if character is "
                    if(line[i-1]!='\\'&&line[i]=='\"'){
                        //Changing flag depending whether characters are still in " " scope
                        if(quotesflag==true){
                            quotesflag=false;
                        }else{
                            quotesflag=true;
                        }
                    //Adding to args if space is found or i==strlen(line) if characters are still in " " scope
                    }else if((line[i]==' '&&quotesflag==false)||((i==strlen(line))&&quotesflag==false)){
                        RemovingSpecialChars(buffer[buffercounter]);
                        args[counter]=buffer[buffercounter];
                        counter++;
                        buffercounter++;
                        charcounter=0;
                    }else{
                        //Adding character in line to buffer
                        buffer[buffercounter][charcounter]=line[i];
                        charcounter++;
                    }
                    //Adding counter when pipe is found
                    if(line[i]=='|')
                        noOfPipes++;
                    
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