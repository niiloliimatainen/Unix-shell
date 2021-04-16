#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void interactive_mode();
void write_error();

int main(int argc, char *argv[]) {

    if (argc == 1) {
        interactive_mode();
    
    } else if (argc == 2) {
        printf("juttuja\n");
   
    } else {
        return 0;
    }







    printf("\n");
    return 0;
}


void interactive_mode() {
    char *buffer = NULL;
    char *token;
    const char delim[1] = " ";
    size_t bufsize = 0;

    while (1) {
        printf("wish> ");
        
        if (getline(&buffer, &bufsize, stdin) == EOF) {
            exit(0);
        }

        /* Removing newline from the end */ 
        buffer[strcspn(buffer, "\n")] = '\0';

        token = strtok(buffer, delim);
/*        while (token != NULL) {
            print("%s\n", token);
            token = strtok()




        }
*/
        if (strcmp("exit", token) == 0) {
            if ((token = strtok(NULL, delim)) != NULL) {
                write_error();
            } else {
                free(buffer);
                exit(0);
            }
        
        } else if (strcmp("cd", token) == 0) {
            printf("huikeit juttui\n");
        
        } else if (strcmp("path", token) == 0) {
            printf("polkujuttuja\n");

        } else {
            printf("juhuu\n");
        }
    }
}


void write_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}



void batch_mode() {

}
/*Get user wanted path as param: f.ex: (current path) + /oskukoodaa/niilokoodaa*/
void cd() {
    
    if (chdir("/PATH THING VARIABLE") != 0){
        printf("Liukastuit banaaniin. Yritäs uudelleen!\n");
        perror("cd failed blaablaa");
    }


}


void path() {

}


void redirect() {

}


