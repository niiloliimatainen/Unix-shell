#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(){
    pid_t parent;
    parent = getpid();
    printf("%d", parent);
    pid_t child = fork();
    printf("%d", child);

    return 0;
}