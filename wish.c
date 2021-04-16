#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc == 1) {
        wish();
    }







    return 0;
}


void wish() {
    while (1) {
        printf("wish>");

    }

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


