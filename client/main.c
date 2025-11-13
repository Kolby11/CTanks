#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "connection/socket.h"

#define PORT 5432
#define BUFF_SIZE 1024

int main() {
    struct sockaddr_in server;
    char buffer[BUFF_SIZE];
    int a = 2, b = 5;

    // Connect to server
    int sock = connect_server("127.0.0.1", PORT, &server);
    if (sock < 0) {
        printf("Failed to connect\n");
        return 1;
    }

    printf("Connected to server on port %d\n", PORT);
    printf("Sending numbers %d and %d\n", a, b);

    // Send data
    sprintf(buffer, "%d %d", a, b);
    send(sock, buffer, strlen(buffer), 0);

    // Receive response
    memset(buffer, 0, BUFF_SIZE);
    recv(sock, buffer, BUFF_SIZE, 0);
    printf("Response from server: %s\n", buffer);

    // Cleanup
    cleanup_socket(sock);

    return 0;
}
