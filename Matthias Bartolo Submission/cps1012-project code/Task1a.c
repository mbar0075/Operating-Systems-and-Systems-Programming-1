#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//Question a)
pid_t fork_exec(char **args);

int main(int argc, char *argv[]) {

    //Executing function fork_exec
   fork_exec(argv);
    return EXIT_SUCCESS;
}

pid_t fork_exec(char **args){
    //Forking process
    pid_t pid = fork();

    // parent process
    if (pid > 0) {
        //Parent process waiting for the child to finish
        if(wait(NULL)==-1){//Executes if wait fails
           perror("wait() failed");
            return pid;
        }
        return pid;
    // child process
    } else if (pid == 0) {
        //Incrementing argument to access the next argument
        ++args;
        //Calling function execvp to execute process
        if(execvp(*args,args)==-1){//Executes if execvp fails
            perror("execvp() failed");
            return pid;
        }
        return pid;
    // error
    } else {
        //Executes if fork fails
        perror("fork() failed");
        return pid;
    }
}
