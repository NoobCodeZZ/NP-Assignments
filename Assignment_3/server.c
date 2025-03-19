#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    char IP_addr[20];
    int port_no;

    // Parse command line arguments
    if(argc != 5)
    {
        printf("Wrong input format\n");
        exit(0);
    }
    else{
        if(strcmp(argv[1],"-i") == 0)
        {
            strcpy(IP_addr,argv[2]);
            printf("IP address: %s\n",IP_addr);
            if(strcmp(argv[3],"-p") == 0)
            {
                port_no = atoi(argv[4]);
                printf("Port number: %d\n", port_no);
            }
        }
        else
        {
            printf("Failed to create server\n");
            exit(0);
        }
    }
    //Creating socket 
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket == -1)
    {
        printf("socket creation on server failed \n");
        exit(0);
    }

    //Initialize server address structure
    struct sockaddr_in serve,client;
    serve.sin_family = AF_INET;
    serve.sin_addr.s_addr = inet_addr(IP_addr);
    serve.sin_port = htons(port_no);

    //Bind the server socket
    if(bind(server_socket,(struct sockaddr *)&serve,sizeof(serve)) != 0)
    {
        printf("socket binding failed\n");
        exit(0);
    }
    
    // listen for incoming connection
    if(listen(server_socket,10)!=0)
    {
        printf("socket failed to listen\n");
        exit(0);
    }

    // accepting connection from client
    // Server will keep running and serving clients until the server process is terminated
    while (1)
    {
        int connection = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&client);
        if (connection < 0)
        {
            perror("server couldn't accept the client");
            exit(0);
        }
        int num1;
        int num2;

        // receiving number from client
        if (recv(connection, &num1, sizeof(num1), 0) == -1)
        {
            printf("Failed to receive integer\n");
            exit(0);
        }
        printf("Number received from client: %d\n", num1);
        printf("Enter second number: ");
        scanf("%d", &num2);
        num1 = num1 + num2;

        // sending sum of received number and number taken as input to the client
        printf("Sending the number: %d\n", num1);
        if (send(connection, &num1, sizeof(num1), 0) == -1)
        {
            printf("Could not send the message\n");
            exit(0);
        }
        close(connection);
    }

    close(server_socket);
    return 0;
}
