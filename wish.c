/*
Sources:
    1. https://brennan.io/2015/01/16/write-a-shell-in-c/
    2. https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file
*/
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAXLEN 64


void interactive_mode();
void write_error(int flag);
void parse_command(char *buffer);
void shell_fork_exec(char **args);
void wish_launch(char **args, int size);
void wish_cd(char **args, int size);
void wish_exit(char *token, char *buffer);
void batch_mode(FILE *file);
FILE * wish_redirect(char *fname);



/* Parsing the arguments and executing them */
int main(int argc, char *argv[]) {
    FILE *file;

    /* If no arguments given, starts an interactive mode */
    if (argc == 1) {
        interactive_mode();
    
    /* If one argument is given, executes that file with batch mode */
    } else if (argc == 2) {

        if ((file = fopen(argv[1], "r")) == NULL) {
                write_error(-1);
			    exit(1);
        } 
        batch_mode(file);
        fclose(file);
   
    /* If more than one argument is give, exit the program */
    } else {
        write_error(0);
        exit(1);
    }
    printf("\n");
    return 0;
}


/* An infinite loop that takes commands from stdin and executes them. The loop stops when "exit" is given. */
void interactive_mode() {
    char *buffer = NULL;
    size_t bufsize = 0;
    
    while (1) {
        printf("wish> ");

        if (getline(&buffer, &bufsize, stdin) != EOF) {
            parse_command(buffer);
        }
    }
    
    free(buffer);
}


/* Reading commands from .wh type of file and executing them */
void batch_mode(FILE *file) {
    char *buffer = NULL;
    size_t bufsize = 0;

    while (getline(&buffer, &bufsize, file) != EOF) {
        parse_command(buffer);
    }

    free(buffer);
}

/* Parsing every command/argument separated with whitespace to args list */
/* Inspiration for args list is taken from 1. source */
void parse_command(char *buffer) {
    char *token, **args, delim[1] = " ";
    int index = 0, size, maxlen = MAXLEN;
    FILE *file = NULL;

    /* Return if input is empty, else remove newline from the end */ 
    if ((size = strcspn(buffer, "\n")) == 0) {
        return;

    } else {
        buffer[size] = '\0';
    }

    token = strtok(buffer, delim);

    /* Own implementation of exit command */
    if (strcmp("exit", token) == 0) {
        if ((token = strtok(NULL, " ")) != NULL) {
            write_error(1);
            return;
        } else {
            free(buffer);
            exit(1);
        }
    }

    /* Allocate memory for args list */
    if ((args = malloc(maxlen * sizeof(char*))) == NULL) {
        write_error(0);
        exit(1);
    }

   /* Looping through commands/arguments and adding them to args list */
    while (1) {
        
        /* If no more commands/arguments, stop the loop */
        if (token == NULL) {
            break;

        } else if (strcmp(">", token) == 0) {

            /* If no output file, the output is written to stdout */
            if ((token = strtok(NULL, delim)) == NULL) {
                break;
            }
            file = wish_redirect(token);
            break;
        }

        args[index] = token;
        index++;

        /* Allocate more memory for args list if needed */
        if (index >= MAXLEN) {
            maxlen += MAXLEN;
            if ((args = realloc(args, maxlen * sizeof(char*))) == NULL) {
                write_error(-1);
                exit(1);
            }
        }
        token = strtok(NULL, delim);
    }

    args[index] = NULL;
    wish_launch(args, index);
    free(args);
    
    /* If there is redirect, close the file and resume standard behavior of stdout */
    if (file != NULL) {
        fclose(file);
        /* This is taken from 2. source */
        freopen("/dev/tty", "w", stdout);
    }
}


 /* Helper function to write errors */
void write_error(int flag) {
    char error_message[MAXLEN];

    /* -1 -> error occured in malloc, realloc or file opening */
    if (flag == -1) {
        strcpy(error_message, "An error has occurred\n");

    /* 0 -> Shell was invoked with more than one batch file */
    } else if (flag == 0) { 
        strcpy(error_message, "Shell can be invoked with only one batch file\n");

    /* 1 -> there was a wrong amount of arguments in exit command */
    } else if (flag == 1) {
        strcpy(error_message, "'exit' command takes no arguments\n");
        
    /* 2 -> there was 0 or >1 arguments in cd command */    
    } else if (flag == 2) {
        strcpy(error_message, "'cd' command takes precisely one argument\n");
    
    /* 3 -> can't find the directory that was given to cd */
    } else if (flag == 3) {
        strcpy(error_message, "No such directory\n");
    
    /*  */
    } else if (flag == 4) {
        strcpy(error_message, "No command is given after ampersand\n");
    
    /* If flag is something else, write universal error message */
    } else {
        strcpy(error_message, "An error has occurred\n");
    }

    write(STDERR_FILENO, error_message, strlen(error_message));
}


/* Built-in command for cd */
void wish_cd(char **args, int size) {
    if (size != 1) {
        write_error(2);
    
    } else {
        /* Check if chdir succeeds */
        if (chdir(args[1]) == -1) {
            write_error(3);
        }
    }
}


void wish_path() {

}


/* Built-in command for redirection. Function takes in filename where stdout is redirected. */
/* Inspiration for freopen is taken from 2. source */
FILE * wish_redirect(char *fname) {
    FILE *file;

    /* Redirecting stdout to fname */
    if ((file = freopen(fname, "w", stdout)) == NULL) {
        write_error(-1);
    }
    return file;
}


/* Function to launch built-in commands or a process */
void wish_launch(char **args, int size){
    char **command;
    int maxlen = MAXLEN, i_args, i_command = 0;

    /*built in testit*/


    /* Allocating memory for command list that stores one command and its arguments at a time */
    if ((command = malloc(maxlen * sizeof(char*))) == NULL) {
        write_error(-1);
        exit(1);
    }

    /* Looping through args list that holds all the commands/arguments from the user's input */
    for (i_args = 0; i_args < size; i_args++) {

        /* If ampersand is found, execute command and its possible arguments */
        if (strcmp("&", args[i_args]) == 0) {
            
            /* If no new command after ampersand, write error and return */
            if (args[i_args + 1] == NULL) {
                write_error(4);
                return;
            }

            shell_fork_exec(command);

            command[i_command + 1] = NULL;

            /* Clearing the command list */
            for (i_command = 0; command[i_command] != NULL; i_command++) {
                command[i_command] = NULL;
            }
            i_command = 0;

        /* Adding command and its possible arguments to command list */
        } else {
            command[i_command] = args[i_args];

            if (i_command >= MAXLEN) {
                maxlen += MAXLEN;
                if ((command = realloc(command, maxlen * sizeof(char*))) == NULL) {
                    write_error(-1);
                    exit(1);
                }
            }
            i_command++;
        }
    }
    /* Executing the last command that was given */
    shell_fork_exec(command);

    pid_t wpid;
    int ret_stat;
    while((wpid = waitpid(-1, &ret_stat, 0)) != -1) {

        if(ret_stat == 0){
            printf("child %d terminated succesfully\n", wpid);
        }else{
            printf("Child %d terminated with error\n", wpid);
            /*Further checks to be added here*/
        }
    }

    free(command);
}


void shell_fork_exec(char **args) {   
    /*NOTE: Now expects only one command: args[] = command, arg1, arg2...*/    
    /*for (int i = 0; i < size; i++) {

        if (strcmp("&", args[i]) == 0){
            counter++;
        }
    }*/
    /*Somewhere here should be a loop to start multiple processes at once*/
    /*Need to figure out if these commands are builtins and should be ran at the main process and not in child*/
    
    /*process part starts*/
    pid_t pid;
    printf("Parent:PID:%d, PPID:%d\n", getpid(), getppid());
    for (int x=0; x<1;x++){
        
    
        
        pid = fork();
        
        /*Fork returns pid of child to parent and pid of 0 to child*/
        /*So child i pid=0 and parent is something else*/
        if (pid == 0){
            printf("PID:%d, PPID:%d\n", getpid(), getppid());
            /*Launch program in child process*/
            /*execvp(prgrm name, arguments vector)*/
            if (execvp(args[0],args) == -1){

                printf("Something happened when launching program %s", args[0]);
                perror("Shell-execute->execvp");
            }
        }else if (pid < 0){
            /*Fork has failed if process id is less than */
            perror("Shell-execute->ParentFork");
    
        }
    }
    /*Now we are in parent process*/

       
    /*process part ends and function can return to receive new calls*/






    /*CASE INTERACTIVE*/
    /* Fork() current process to create a copy
       to know which process is which, we look at the process id's
       -> parent has child_pid of child and child has child_pid of 0
       
       use exec variant execvp() (or something) to run wanted program in child process created by fork
       exec + v(takes vector of parameters) p (finds program by its name)

       parent waits until child has completed
       add a shitton of if's and error checks and validate that child process has ended properly
       after that we can go on and start doing other stuff
    */
   /*CASE BATCH*/
   /*For i in commands i*/

   
    printf("\n");
    return;

}

