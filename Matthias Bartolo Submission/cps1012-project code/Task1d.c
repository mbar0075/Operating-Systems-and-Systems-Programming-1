#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int execute_pipeline_async( char ** pipeline_args [],bool async);

int main(int argc, char *argv[]) {
    //Pipeline arguments
    char*args1[]= {"ls", "-la", NULL};
    char*args2[]= {"grep","matthias", NULL};
    char*args3[]= {"wc","-l", NULL};

    //Assigning arguments to pipeline array
    char ** pipeline [] = { args1, args2, args3 , NULL };
    //Executing function execute_pipeline
    int result = execute_pipeline_async( pipeline ,true);

    return EXIT_SUCCESS;
}


//Question d)
int execute_pipeline_async( char ** pipeline_args [],bool async){
    //pid to be used for forking
    pid_t pid;
    //Pipe pointers
    int fd[2];
    //Variable to be overwritten to hold the output file location pertaining to pipe
    int tmp_fd=0;

    //Loops through all the pipeline array
    while(*pipeline_args!=NULL){
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
            //Assigning the standard input to the the pipe output(tmp_fd)
            dup2(tmp_fd,STDIN_FILENO);
            //Checking if last process is reached i.e. last command
            if(*(pipeline_args+1)!=NULL){
                //Assigning the standard output to the the pipe input(tmp_fd)
                dup2(fd[1],STDOUT_FILENO);
            }
            //Closing the read end of pipe
            close(fd[0]);
            //Executing pipeline arguments
            if(execvp(*(pipeline_args)[0],*pipeline_args)==-1){//Executes if execvp fails
                perror("execvp() failed");
                exit(EXIT_FAILURE);
            }
        }//Parent Process
        else{
           
            //Closing the write end of pipe
            close(fd[1]);
            //Setting output of pipe to tmp_fd
            tmp_fd=fd[0];
            //Increment pointer to access the next set of arguments
            pipeline_args++;
        }
    }
    //Making the main process to wait for the final child process
    if(async==true){
        printf("Parent is waiting\n");
        int status;
        // wait for termination of last pipeline stage
        waitpid(pid,&status,0);
        printf("Parent stopped waiting\n");
    }
    //Closing the read end of pipe
    close(fd[0]);

}
