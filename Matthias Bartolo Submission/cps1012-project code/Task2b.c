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
    char dir[100];
    printf("%s\n", getcwd(dir, 100));
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


int main(int argc, char *argv[]) {

    if(argc>1){//To check if there is more than argument
        //Copying all the arguments in another array excluding the first element
        char *args[argc-1];
        for(int i=0;i<argc-1;i++){
           
            args[i]=argv[i+1];
        }

        for( int i=0; i<4;i++){//Looping through array of builtin commands
            //Comparing argument 1 with the builtin commands and executes said command if they match
            if(strncmp(argv[1],builtin_list[i].name,strlen(argv[1]))==0){
                builtin_list[i].method(args);
                exit(EXIT_SUCCESS);
            }
        }
    }

    printf("Invalid command\n");
    exit(EXIT_SUCCESS);

}
