#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    char IP_addr[20];
    int port_no;
    // Checks no. of command line arguments passed in the program
    if(argc != 5)
    {
        printf("Wrong input format\n");
        exit(0);
    }
    else{
        if(strcmp(argv[1],"-s") == 0)
        {
            strcpy(IP_addr,argv[2]);
            if(strcmp(argv[3],"-p") == 0)
            {
                port_no = atoi(argv[4]);
            }
        }
        else
        {
            printf("Failed to create server\n");
            exit(0);
        }
    }
    // Creating a new socket
    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket == -1)
    {
        printf("Error creating socket\n");
        exit(0);
    }
    struct sockaddr_in server, client_address;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_addr);
    // Converting from host byte order to network byte order
    server.sin_port = htons(port_no);
    int len = sizeof(server);
    // Connecting to the server
    int connection = connect(client_socket,(struct sockaddr *)&server,sizeof(server));
    if(connection == -1)
    {
        printf("Error connecting to server\n");
        exit(0);
    }
    socklen_t client_address_length = sizeof(client_address);
    // Printing port no. of the client
    if(getsockname(client_socket, (struct sockaddr *)&client_address, &client_address_length)){
        perror("getsockname");
        return 1;
    }
    printf("Client's port numbber: %d\n", ntohs(client_address.sin_port));
    
    // Sending number to the server 
    int numb;
    printf("Enter a number: ");
    scanf("%d",&numb);
    if(send(client_socket,&numb,sizeof(int),0) == -1)
    {
        printf("The message could not be sent\n");
        exit(0);
    }
    int numb2;

    // Receiving sum from the server
    if(recv(client_socket,&numb2,sizeof(int),0) == -1)
    {
        printf("Error receiving data\n");
        exit(0);
    }
    printf("Number received from server: %d\n",numb2);
    close(client_socket);
    return 0;
}
