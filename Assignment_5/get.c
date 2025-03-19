#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void get_html(const char *url) {
    char *hostname;
    char *path = "/";
    char request[BUFFER_SIZE];

    // Parse URL to extract hostname and path
    if (strstr(url, "http://") != NULL) {
        hostname = strdup(url + strlen("http://"));
    } else {
        hostname = strdup(url);
    }

    char *path_start = strchr(hostname, '/');
    if (path_start != NULL) {
        *path_start = '\0';
        path = path_start;
    }

    // Resolve server hostname to IP address
    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error: Could not resolve hostname\n");
        exit(EXIT_FAILURE);
    }

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length); // Use h_addr_list instead of h_addr
    server_addr.sin_port = htons(80);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error connecting to server");
    }

    // Construct HTTP GET request
    snprintf(request, BUFFER_SIZE, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, hostname);

    // Send HTTP request to server
    if (write(sockfd, request, strlen(request)) < 0) {
        error("Error writing to socket");
    }

    // Get HTML content
    char response[BUFFER_SIZE];
    FILE *file = fopen("index.html", "w");
    if (file == NULL) {
        error("Error opening file");
    }

    int bytes_received;
    while ((bytes_received = read(sockfd, response, BUFFER_SIZE)) > 0) {
        fwrite(response, 1, bytes_received, file);
    }

    if (bytes_received < 0) {
        error("Error reading from socket");
    }

    fclose(file);
    close(sockfd);

    free(hostname);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return 1;
    }

    char *url = argv[1];
    get_html(url);

    printf("HTML content saved as index.html\n");

    return 0;
}
