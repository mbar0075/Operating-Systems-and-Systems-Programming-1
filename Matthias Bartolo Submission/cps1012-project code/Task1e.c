#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int execute_pipeline_async( char ** pipeline_args [],bool async,char * file_in , char * file_out , bool append_out);

int main(int argc, char *argv[]) {
    //Pipeline arguments
    char*args1[]= {"cowsay","Test", NULL};
    char*args2[]= {"cat", NULL};

    //Assigning arguments to pipeline array
    char ** pipeline [] = { args1,args2, NULL };
    //Executing function execute_pipeline_async
    int result = execute_pipeline_async( pipeline ,true,NULL,NULL,false);

    return EXIT_SUCCESS;
}


//Question e)
int execute_pipeline_async( char ** pipeline_args [],bool async,char * file_in , char * file_out , bool append_out){
       //pid to be used for forking
    pid_t pid;
    //Pipe pointers
    int fd[2];
    //Variable to be overwritten to hold the output file location pertaining to pipe
    int tmp_fd=0;
    //To get the number of arguments in pipeline_args which will be used for output redirection
    int noofargs=0;
    while(pipeline_args[noofargs]!=NULL){
        noofargs++;
    }
    //Setting counter to 0
    int counter=0;
    

    while(pipeline_args[counter]!=NULL){
       //Creating a pipe
        if (pipe(fd) < 0) {//Executes if pipe fails
            perror("pipe() failed");
            exit(EXIT_FAILURE);
        }//Forking process
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
            //Incrementing counter to access the next set of arguments
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
