# Operating Systems and Systems Programming 1

# Author
**Matthias Bartolo 0436103L**

## Preview:
![TISH](https://github.com/mbar0075/Operating-Systems-and-Systems-Programming-1/assets/103250564/bec44cbd-8012-4afe-9041-24663147a07d)

## Description of Task:
The purpose of this project involved the creation of a **Tiny Shell (tish$>)**, which would have the functionality of executing multiple builtin commands and pipeline commands. The shell included various error checking capabilities, the functionality of executing multiple command pipelines, and accepting text and meta characters which were enclosed in quotes.

### Conceptual Overview of Tiny Shell:
**The implementation of the shell included the following structure:** <br>

1. On start-up the shell will wait for user input <br>
2. The user inputs the input to the terminal <br>
3. The user input is checked for multiple command pipelines in a single input, and the shell will separate the multiple pipelines into individual pipelines (delimited by the ; metacharacter). <br>
4. The individual pipeline is then checked for Unmatched quotes, if the individual pipeline has an odd number of quotes, then an error is presented, and the shell will resort to executing the next individual pipeline, else it will continue to execute the current individual pipeline. <br>
5. The current individual pipeline is escaped for quotes and backslash characters, preceding by a backslash. The shell also tokenises the individual pipeline into individual arguments which are delimited by a single or multiple spaces. Text which is enclosed inside the quotes scope is taken as a single argument, such text may include meta characters and spaces. <br>
6. The shell then checks if the individual arguments, match any builtin commands, if so,it will resort to exit said commands and continue execution of the next individual pipeline. Else it will continue executing the current individual pipeline. Note that if the exit builtin is executed, the shell will terminate. <br>
7. Subsequently, the shell checks for errors pertaining to the |, >>, >, < metacharacters, if an error is found, the relevant error is displayed to the user, and the shell will proceed to execute the next individual pipeline. Else it will continue executing the current individual pipeline. <br>
8. The shell will proceed to execute an external pipeline command, and will either execute successfully, or an error occurs during execution, either way, the shell will resort to execute the next individual pipeline. <br>
9. The shell continues to iterate over the individual pipelines, until last individual pipeline is executed, then it will repeat from step 1.<br>
10. The shell will terminate if the user inputs the exit command.<br><br>


<p align='center'>
  <img src="https://github.com/mbar0075/Operating-Systems-and-Systems-Programming-1/assets/103250564/e9c679f4-0fd5-4b8e-9141-592b1ef61e17" style="display: block; margin: 0 auto; width: 50%; height: auto;"><br>
</p>

### Creation of Tiny Shell:
**Creation of the aformentioned Tiny Shell (tish$>) can be partioned in the following tasks:**  <br>

The first task involved **Process Control**, which served as the foundation of Tiny Shell and accounted for a significant portion of the assignment evaluation. The primary focus was on building a framework to launch external programs. Thorough testing of the implementation was emphasized to save time during the debugging phase of subsequent tasks. The problems in this task included launching executables using the fork-exec pattern, enabling concurrent execution of two programs through a pipe, supporting arbitrary-sized pipelines by handling a variable number of concurrent programs, establishing a producer-consumer chain between stages using pipes, providing an option to block the parent's execution until the pipeline completes, and implementing conditional input and output redirection to files.

```c
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
```

Another task addressed the handling of **Shell Builtin Commands**, which accounted for a specific percentage of the assignment evaluation. This task built upon the foundation established in the previous task and focused on executing builtin commands within the shell. Builtin commands are internal to the shell and do not rely on external programs. The problems in this task involved implementing a mechanism to map builtin names to corresponding functions, allowing execution based on the input command. Specific builtin commands, such as "exit" for program termination, "cd" for changing the current working directory, "cwd" for displaying the current working directory, and "ver" for providing information about the Tiny Shell program, were required to be implemented.

<p align='center'>
  <img src="https://github.com/mbar0075/Operating-Systems-and-Systems-Programming-1/assets/103250564/25968ec2-8e75-4eff-a785-51cd6b11011b" style="display: block; margin: 0 auto; width: 45%; height: auto;">
</p>

Furthermore, **Terminal Input and Output** were addressed in a separate task. This task concentrated on creating the command line interface, handling user input, and managing output to the terminal. The problems in this task included reading a single line of text from standard input, tokenizing the input based on spaces, detecting metacharacters for input/output redirection and command pipeline creation, executing command pipelines by attempting to execute builtin commands first and then external command pipelines in blocking mode, modifying the program to continuously read input and execute commands until the user initiates the exit command, and displaying a prompt to indicate that the shell is ready to receive input.

```c
pipeline[pipelinecounter]=arguments[pipelinecounter];
pipeline[pipelinecounter+1]=NULL;
int result = execute_pipeline_async( pipeline ,true,file_in,file_out,append_out,noOfPipes);
```

Lastly, an **Advanced Scanning** task was introduced to improve the expressiveness of the command input scanner. The aim was to enhance the tokenization process by adding rules. These rules involved using quotation marks to change the interpretation of whitespace and other metacharacters within the quoted string, introducing an exception for the backslash character to remove the special meaning of certain metacharacters, and using the semicolon as a metacharacter to separate multiple command pipelines on a single line.

```c
 while((line = linenoise("tish$>")) != NULL) {
        
        /* Do something with the string. */
        if (line[0] != '\0' ) {
            linenoiseHistoryAdd(line); /*Add to the history.*/
            linenoiseHistorySave("history.txt"); /*Save the history on disk. */ 
            
          .  .  . 
        }
  .  .  .
 }
```

## Deliverables:
The repository includes The Operating System Assignment (Building a Tiny Shell/CLI)<br />

