#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
int main()
{
    pid_t pid1,pid2;
    int fd[2], fd2[2], fd3[2],fd4[2];
    int bytesRead,bytesRead2,bytesRead3,bytesRead4;
    pipe(fd);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    int message,message2,message3,message4;
    int message_r,message2_r,message3_r,message4_r;
    pid1 = fork();
    switch(pid1)
    {
        case -1:
            perror("fork");
            exit(1);

        case 0 :
            sleep(5);
            printf("\nThis is the Child process : pid=%d\n", getpid());
            close(fd[WRITE]);
            // the child process is reading the integer sent through fd
            bytesRead = read(fd[READ], &message_r,sizeof(int));
            printf("Child received integer : %d, from the parent process\n", message_r);
            printf("User input to the child : ");
            scanf("%d", &message2);
            message2 += message_r;
            // Creating the grandchild process
            pid2 = fork();
            switch(pid2)
            {
                case - 1:
                    perror("fork");
                    exit(1);
                case 0:
                    printf("\nThis is the Grandchild process : pid = %d \n", getpid());
                    close(fd2[WRITE]);
                    close(fd3[READ]);
                    int message2_r;
                    bytesRead2 = read( fd2[READ], &message2_r,sizeof(message2_r));
                    printf("Grandchild received integer : %d, from the child process\n", message2_r);
                    close(fd2[READ]);
                    // Taking input from the user, adding it and sending back to the child
                    printf("User input to the grandchild : ");
                    scanf("%d", &message3);
                    message3+= message2_r;
                    write(fd3[WRITE], &message3,sizeof(int));
                    printf("This is the grandchild process : pid = %d exiting \n", getpid());
                    exit(0);
                default : 
                    close(fd2[READ]);
                    close(fd3[WRITE]);
                    //Taking input from the user and writing it to the grandchild(sum of previous 2 values)
                    write(fd2[WRITE], &message2, sizeof(int));
                    close(fd2[WRITE]);
                    bytesRead3 = read( fd3[READ], &message3_r,sizeof(message3_r));
                    printf("\nThis is the child process : pid = %d\n", getpid());
                    printf("Integer received from grandchild : %d \n", message3_r);
                    close(fd3[READ]);
                    close(fd4[READ]);
                    // Sending the value received from grandchild back to parent
                    write(fd4[WRITE], &message3_r,sizeof(int));
                    close(fd4[WRITE]);
                    printf("This is the child process : pid = %d, exiting \n", getpid());
                    exit(0);
            }
 
        default :
            printf("This is the Parent process: pid=%d \n", getpid() );
            printf("User Input to the parent : ");
            scanf("%d", &message);

            close(fd[READ]);
            write(fd[WRITE], &message,sizeof(message));
            close(fd[WRITE]);
            wait(NULL);
            // Taking input from the child(fd4) and printing it
            close(fd4[WRITE]);
            bytesRead4 = read( fd4[READ], &message4_r,sizeof(message4_r));
            printf("\nThis is the parent process : pid = %d\n", getpid());
            printf("Integer received from child : %d \n", message4_r);
            close(fd4[READ]);
            printf("This is the parent process : pid = %d exiting\n", getpid());
            exit(0);
    }
}