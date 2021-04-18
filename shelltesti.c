    /*NOTE: Now expects only one command: args[] = command, arg1, arg2...*/
    int counter = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp("&", args[i]) == 0){
            counter++;
        }
    }
    printf("counter:%d\n", counter);
    /*Somewhere here should be a loop to start multiple processes at once*/
    /*Need to figure out if these commands are builtins and should be ran at the main process and not in child*/
    
    /*process part starts*/
    pid_t pid, wpid;

    pid = fork();
    /*Fork returns pid of child to parent and pid of 0 to child*/
    /*So child i pid=0 and parent is something else*/
    if (pid == 0){
        /*Launch program in child process*/
        /*execvp(prgrm name, arguments vector)*/
        if (execvp(args[0],args) == -1){
            fprintf("Something happened when launching program %s", args[0]);
            perror("Shell-execute->execvp");
        }

    }else if (pid < 0){
        /*Fork has failed if process id is less than */
        perror("Shell-execute->ParentFork");
    }else{
        /*Now we are in parent process*/
        int ret_stat;
        waitpid(pid, &ret_stat, 0);
        if(ret_stat == 0){
            printf("child terminated succesfully\n");
        }
        else{
            printf("Child terminated with error");
            /*Further checks to be added here*/
        }
        
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