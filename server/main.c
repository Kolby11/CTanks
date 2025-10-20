#include <stdio.h>
#include <string.h>
#include "connection/socket.h"

#define PORT 5432
#define BUFF_SIZE 1024

int main() {
    struct sockaddr_in server, client;
    char buffer[BUFF_SIZE];
    
    printf("=== Server starting ===\n");
    
    // Initialize network
    if (!initialize_network()) {
        printf("Failed to initialize network\n");
        return 1;
    }
    printf("Network initialized\n");
    
    // Create server socket
    socket_t serverSock = create_server_socket(PORT, &server);
#ifdef _WIN32
    if (serverSock == INVALID_SOCKET) {
        printf("Failed to create server socket\n");
        cleanup_network(serverSock);
        return 1;
    }
#else
    if (serverSock < 0) {
        printf("Failed to create server socket\n");
        cleanup_network(serverSock);
        return 1;
    }
#endif
    
    printf("Server is ready. Listening on port %d\n", PORT);
    printf("Waiting for client connection...\n");
    
    // Accept client
    socket_t clientSock = accept_client(serverSock, &client);
#ifdef _WIN32
    if (clientSock == INVALID_SOCKET) {
        printf("Failed to accept client\n");
        cleanup_network(serverSock);
        return 1;
    }
#else
    if (clientSock < 0) {
        printf("Failed to accept client\n");
        cleanup_network(serverSock);
        return 1;
    }
#endif
    
    // Get client IP
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    printf("Client connected from %s:%d\n", clientIP, ntohs(client.sin_port));
    
    // Communication loop
    while (1) {
        memset(buffer, 0, BUFF_SIZE);
        
        // Receive data from client
        int bytesReceived = recv(clientSock, buffer, BUFF_SIZE - 1, 0);
        
        if (bytesReceived <= 0) {
            if (bytesReceived == 0) {
                printf("Client disconnected\n");
            } else {
                printf("Receive error\n");
            }
            break;
        }
        
        printf("Received from client: %s\n", buffer);
        
        // Echo back to client
        int bytesSent = send(clientSock, buffer, bytesReceived, 0);
        if (bytesSent <= 0) {
            printf("Send error\n");
            break;
        }
        
        printf("Echoed back to client\n");
    }
    
    // Cleanup
    printf("Closing connections...\n");
#ifdef _WIN32
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
#else
    close(clientSock);
    close(serverSock);
#endif
    
    printf("Server stopped\n");
    return 0;
}