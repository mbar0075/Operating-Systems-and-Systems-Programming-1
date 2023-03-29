#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "linenoise.h"
#define SIZE 124


//Task 1
//Executing pipeline
int execute_pipeline_async( char ** pipeline_args [],bool async,char * file_in , char * file_out , bool append_out, int noofargs){    
    //pid to be used for forking
    pid_t pid;
    //Pipe pointers
    int fd[2];
    //Variable to be overwritten to hold the output file location pertaining to pipe
    int tmp_fd=0;
    //Setting counter to 0
    int counter=0;


    while(pipeline_args[counter]!=NULL){
       //Creating a pipe
        if (pipe(fd) < 0) {//Executes if pipe fails
            perror("pipe() failed");
            exit(EXIT_FAILURE);
        } 
        //Forking process
         if((pid=fork())<0){//Executes if fork fails
            perror("fork() failed");
            exit(EXIT_FAILURE);
        }//Child process
        else if(pid==0){
            //i)Input redirection
            if(file_in !=NULL && counter==0){
                freopen(file_in,"r",stdin);
            }
            //Assigning the standard input to the the pipe output(tmp_fd)
            dup2(tmp_fd,STDIN_FILENO);
            //Checking if last process is reached i.e. last command
            if(pipeline_args[counter+1]!=NULL){
                //Assigning the standard output to the the pipe input(tmp_fd)
                dup2(fd[1],STDOUT_FILENO);
            }
            //ii)Output redirection
            if(file_out!=NULL && counter==(noofargs-1)){
                if(append_out==true){
                    //Appending to specified file
                    freopen(file_out,"a",stdout);
                }
                else{
                    //Writing to specified file
                    freopen(file_out,"w",stdout);
                }
            }
            //Closing the read end of pipe
            close(fd[0]);
            //Executing pipeline arguments
            if(execvp(*pipeline_args[counter],pipeline_args[counter])==-1){//Executes if execvp fails
                perror("execvp() failed");
                exit(EXIT_FAILURE);
            }
        }//Parent Process
        else{
           
            //Closing the write end of pipe
            close(fd[1]);
            //Setting output of pipe to tmp_fd
            tmp_fd=fd[0];
            //Incrementing counter
            counter++;
        }
    }
     //Making the main process to wait for the final child process
    if(async==true){
        int status;
        // wait for termination of last pipeline stage
        waitpid(pid,&status,0);
    }
    //Closing the read end of pipe
    close(fd[0]);
}


//Task 2
typedef int (* builtin_t )( char **);

//Struct for a builtin command
struct builtin_command {
    char * name;
    builtin_t method ;
};

//Executable for exit command
int exit_code ( char ** args ) {
    //execute builtin code
    printf("GoodBye\n");
    exit(EXIT_SUCCESS);
}

//Executable for cd command
int cd_code ( char ** args ) {
    //execute builtin code
    chdir(args[1]);
    return 0;
}

//Executable for cwd command
int cwd_code ( char ** args ) {
    //execute builtin code
    char dir[SIZE];
    printf("%s\n", getcwd(dir, SIZE));
    return 0;
}

//Executable for ver command
int ver_code ( char ** args ) {
    //execute builtin code
    printf("\n\n#--------------------#---------------------#\n");
    printf("| author:\033[0;34m Matthias\033[0m           version:\033[0;34m 1.7\033[0m  |\n");
    printf("|                                          |\n");
    printf("| [You have met all of the requirements    |\n");
    printf("|     to complete the secret quest:        |\n");
    printf("#       \033[0;35m \"COURAGE OF THE WEAK\"\033[0m]            #\n" );
    printf("|                                          |\n");
    printf("| [Congratulations! You have now become a  |\n");
    printf("|               \033[0;32m \"PLAYER\"\033[0m]                 |\n" );
    printf("|                                          |\n");
    printf("#--------------------#---------------------#\n\n");
    return 0;
}

//Array of builtin commands
struct builtin_command builtin_list [] = {
{ "exit" , & exit_code },
{ "cd" , & cd_code },
{ "cwd" , & cwd_code },
{ "ver" , & ver_code }
};


//Task 3
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

//Task 4
//Method for removing \ & "
void RemovingSpecialChars(char* input){
    //Calculating length of input
    int len=0;
    for(len=0;input[len];len++);

    //Looping through all elements input
    for(int i=0;i<len;i++){
        //if \" or \\ is found then move all elements to the left
        if(input[i]=='\\'&&(input[i+1]=='\"'||input[i+1]=='\\')){
            for(int j=i;j<len;j++){
                input[j]=input[j+1];
            }
            len--;
            i--;
        }
    }
}