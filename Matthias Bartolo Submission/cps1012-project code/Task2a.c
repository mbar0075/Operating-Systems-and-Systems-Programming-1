#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef int (* builtin_t )( char **);

//Struct for a builtin command
struct builtin_command {
    char * name;
    builtin_t method ;
};

int builtin_code1 ( char ** args ) {
    //execute builtin code
    printf("Hello\n");
    return 0;
}

int builtin_code2 ( char ** args ) {
    //execute builtin code
    printf("Bye\n");
    return 0;
}

//Array of builtin commands
struct builtin_command builtin_list [] = {
{ "welcome" , & builtin_code1 },
{ "exit" , & builtin_code2 },
};


int main(int argc, char *argv[]) {
    if(argc>1){//To check if there is more than argument
        for( int i=0; i<2;i++){//Looping through array of builtin commands
            //Comparing argument 1 with the builtin commands and executes said command if they match
            if(strncmp(argv[1],builtin_list[i].name,strlen(argv[1]))==0){
                builtin_list[i].method(argv);
                exit(EXIT_SUCCESS);
            }
        }
    }

    printf("Invalid command\n");
    exit(EXIT_SUCCESS);
    
        
}