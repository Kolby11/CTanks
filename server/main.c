#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define close closesocket
    typedef SOCKET socket_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int socket_t;
#endif


#define PORT 5432
#define BUFF_SIZE 1024

int main() {
    struct sockaddr_in server, client;
    char buffer[BUFF_SIZE];
    int a, b, result;
    
    // Initialize network
    if (!initialize_network()) {
        return 1;
    }
    
    // Create server socket
    socket_t serverSock = create_server_socket(PORT, &server);
    #ifdef _WIN32
        if (serverSock == INVALID_SOCKET) {
            cleanup_network(serverSock);
            return 1;
        }
    #else
        if (serverSock < 0) {
            cleanup_network(serverSock);
            return 1;
        }
    #endif
    
    printf("Server je pripraveny. Pocuvam na porte %d\n", PORT);
    
    // Accept client
    socket_t clientSock = accept_client(serverSock, &client);
    #ifdef _WIN32
        if (clientSock == INVALID_SOCKET) {
            leanup_network(serverSock);
            return 1;
        }
    #else
        if (clientSock < 0) {
            cleanup_network(serverSock);
            return 1;
        }
    #endif
    
    printf("Klient sa pripojil\n");
    
    // Receive data from client
    memset(buffer, 0, BUFF_SIZE);
    int bytesReceived = recv(clientSock, buffer, BUFF_SIZE, 0);
    if (bytesReceived > 0) {
        printf("Prijate data: %s\n", buffer);
        
        // Parse the two integers
        sscanf(buffer, "%d %d", &a, &b);
        printf("Cisla: a=%d, b=%d\n", a, b);
        
        // Calculate result
        result = add(a, b);
        printf("Vysledok: %d + %d = %d\n", a, b, result);
        
        // Send result back to client
        sprintf(buffer, "%d", result);
        send(clientSock, buffer, strlen(buffer), 0);
        printf("Vysledok odoslany klientovi\n");
    }
    
    // Cleanup
    cleanup_network(clientSock);
    cleanup_network(serverSock);
    
    return 0;
}