#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
 
int main() {
    pid_t pid1,pid2;
 
    pid1 = fork();
    switch(pid1)  {
        case -1:
            perror("fork");
            exit(1);
        case 0: // First child process
            sleep(1);
            printf("This is the Child process: pid=%d\n", getpid());
            sleep(1);
            pid2 = fork();
            sleep(1);
            switch(pid2){
                case -1:
                    perror("fork");
                    exit(1);
                case 0:
                    printf("This is the Grandchild process : pid = %d \n", getpid());
                    sleep(1);
                    printf("The grandchild process is exiting \n");
                    exit(0);
                default :
                    wait(0);
                    printf("This is the Child process. The Grandchild process : pid = %d has exited \n", pid2 );
            }
            sleep(1);
            printf("The child process is exiting \n");
            exit(0);

        default:
            printf("This is the Parent process: pid=%d \n", getpid() );
            wait(0);
   
            printf("This is the Parent process. The Child process : pid=%d has exited \n", pid1 );
            sleep(1);
            printf("This is the Parent process: pid=%d \n exiting", getpid() );
            exit(0);
 
    }
    return 0;
}
 