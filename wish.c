/****************************************************************/

/* Creators: Niilo Liimantainen & Oskari Kivekäs */
/* Date: 20.04.2021 */
/* Sources: */
/*   1. https://brennan.io/2015/01/16/write-a-shell-in-c/ */
/*   2. https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file */
    
/****************************************************************/


#include "wish.h"


/* Parsing the arguments and executing them */
int main(int argc, char *argv[]) {
    FILE *file;

    /*INIT path enviroment variable*/
    char path[PATH_MAX] ="PATH=";
    if(putenv(strcat(path, "/bin/")) != 0){
        write_error(-1);
        exit(1);
    }

    /* If no arguments given, starts an interactive mode */
    if (argc == 1) {
        interactive_mode();
    
    /* If one argument is given, executes that file with batch mode */
    } else if (argc == 2) {

        if ((file = fopen(argv[1], "r")) == NULL) {
                write_error(7);
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

    /* Call wish_exit if the command is exit without arguments */
    if (strcmp("exit", token) == 0) {
        if ((token = strtok(NULL, " ")) != NULL) {
            write_error(1);
            return; 
        }
        wish_exit(buffer);
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

        /* Call for redirect if '>' is found */
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


/* Function to launch built-in commands or a process */
void wish_launch(char **args, int size) {
    char **command;
    int maxlen = MAXLEN, i_args, i_command = 0;

    /* Test if command is built-in */
    if (strcmp("cd", args[0]) == 0) {
        wish_cd(args, size);
        return;
    
    } else if (strcmp("path", args[0]) == 0) {
        wish_path(args, size);
        return;
    }

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
            
            /* If command after ampersand is built-in (except redirection), write error and return */
            } else if (strcmp("cd", args[i_args + 1]) == 0) {
                write_error(5);
                return;
            
            } else if (strcmp("path", args[i_args + 1]) == 0) {
                write_error(5);
                return;
            
            } else if (strcmp("exit", args[i_args + 1]) == 0) {
                write_error(5);
                return;
            } 

            wish_fork_exec(command);
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
    wish_fork_exec(command);
    /*When all commands have been send to children, main process waits here!*/
    pid_t wpid;
    int ret_stat;
    while((wpid = wait(&ret_stat)) != -1) {

        if(ret_stat == 0){
            /*We are ok, child terminated with success*/
        }else{
            
            /*In case child terminated with error*/
            
        }
    }

    free(command);
}


/*check for access before execution*/
const char* check_path(char* prog_name) {
    char p[PATH_MAX], *token, *p_path = getenv("PATH");
    const char* path = p;
    int a;

    /*split path by whitespace and add program name to path before checking access*/    
    while ((token = strtok_r(p_path, " ", &p_path))) {
        
        /*access() and execv() require const char*, so we create path and change it to point p*/
        strcpy(p, token);
        strcat(p, prog_name);
        
        /*if path has wrx permissions, we return that path executor function*/
        a = access(path, X_OK);
        if (a == 0) {
            return path;
        }
    }
       
    return "NOPATH";
}


/* Function to execute non-built-in commands */
void wish_fork_exec(char **args) {   

    /*Check for valid path*/
    const char* path = check_path(args[0]);
    
    if (strcmp(path, "NOPATH") == 0){
        write_error(6);
        return;
    }
    /*Fork a process to execute wanted program*/
    pid_t pid;
    pid = fork();
    
    /*Fork returns pid of child to parent and pid of 0 to child*/
    if (pid == 0) {
        /*Launch program in child process*/
        /*execv(prgrm path, arguments vector)*/
        if (execv(path,args) == -1) {
            printf("Something happened when launching program %s", path);
            perror("Wish_execv");
            exit(1);
        }

    } else if (pid < 0) {
        /*Fork has failed if process id is less than 0*/
        perror("Wish_execv");
        exit(1);
    }
    /*Main process returns here since it has nothing to do after fork*/
    return;
}
