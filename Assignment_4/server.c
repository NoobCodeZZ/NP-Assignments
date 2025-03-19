#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

struct ThreadArgs {
    int client_socket;
    int num2; 
};

void *handle_client(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int client_socket = args->client_socket;
    int num2 = args->num2;
    free(args); 

    int num1;

    if (recv(client_socket, &num1, sizeof(num1), 0) == -1) {
        printf("Failed to receive integer\n");
        close(client_socket);
        pthread_exit(NULL);
    }
    printf("Number received from client: %d\n", num1);

    num1 += num2;

    printf("Sending the number: %d\n", num1);
    if (send(client_socket, &num1, sizeof(num1), 0) == -1) {
        printf("Could not send the message\n");
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    char IP_addr[20];
    int port_no;
    int num2;
    if (argc != 7) {
        printf("Wrong input format\n");
        exit(0);
    } else {
        if (strcmp(argv[1], "-i") == 0) {
            strcpy(IP_addr, argv[2]);
            printf("IP address: %s\n", IP_addr);
            if (strcmp(argv[3], "-p") == 0) {
                port_no = atoi(argv[4]);
                printf("Port number: %d\n", port_no);
            }

            if(strcmp(argv[5],"-c") == 0)
            {
                num2 = atoi(argv[6]);
            }
        } else {
            printf("Failed to create server\n");
            exit(0);
        }
    }
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("socket creation on server failed \n");
        exit(0);
    }
    struct sockaddr_in serv, client;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(IP_addr);
    serv.sin_port = htons(port_no);
    if (bind(server_socket, (struct sockaddr *)&serv, sizeof(serv)) != 0) {
        printf("socket binding failed\n");
        exit(0);
    }

    if (listen(server_socket, 10) != 0) {
        printf("socket failed to listen\n");
        exit(0);
    }

    while (1) {
        int client_socket;
        pthread_t tid;
        struct ThreadArgs *args;

        client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&client);
        if (client_socket < 0) {
            printf("server couldn't accept the client");
            exit(0);
        }

        // Create arguments structure
        args = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
        if (args == NULL) {
            perror("Memory allocation failed");
            close(client_socket);
            continue; 
        }
        args->client_socket = client_socket;
        args->num2 = num2; 
        
        // Create thread to handle client
        if (pthread_create(&tid, NULL, handle_client, (void *)args) != 0) {
            printf("Failed to create thread for client\n");
            free(args);
            close(client_socket);
            continue; 
        }

        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}