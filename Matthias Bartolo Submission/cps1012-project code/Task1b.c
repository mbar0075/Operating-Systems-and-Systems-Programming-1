#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t fork_exec_pipe(char **args1,char **args2);

int main(int argc, char *argv[]) {
    //pipeline arguments
    char*args1[]= {"ls", "-l", NULL};
    char*args2[]= {"wc","-l", NULL};
    
    //Executing function fork_exec_pipe
    fork_exec_pipe(args1,args2);
    return EXIT_SUCCESS;
}


pid_t fork_exec_pipe(char **args1,char **args2){
    //Child process pids
    pid_t pid1,pid2;
    //Pipe pointers
    int fd[2];

    //Creating a pipe
    if (pipe(fd) < 0) {//Executes if pipe fails
        perror("pipe() failed");
        exit(EXIT_FAILURE);
    }

    //Forking first child process
    if((pid1=fork())<0){
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }//Child process
    else if(pid1==0){
        //Closing the read end of pipe
        close(fd[0]);
        //Assigning the standard output to the the pipe input
        dup2(fd[1],STDOUT_FILENO);
        //Executing program
        if(execvp(*args1,args1)==-1){//Executes if execvp fails
            perror("execvp() failed");
            exit(EXIT_FAILURE);
        }
    }
    //Forking second child process
    if((pid2=fork())<0){//Executes if fork fails
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }//Child process
    else if(pid2==0){
        //Closing the write end of pipe
        close(fd[1]);
        //Assigning the standard input to the the pipe output
        dup2(fd[0],STDIN_FILENO);
        //Executing program
        if(execvp(*args2,args2)==-1){//Executes if execvp fails
            perror("execvp() failed");
            exit(EXIT_FAILURE);
        }
    }
    // parent process closes both pipe ends
    close(fd[0]);
    close(fd[1]);
    // wait for termination of last pipeline stage
    int status;
    waitpid(pid2, &status, 0);
    return pid2;
}
