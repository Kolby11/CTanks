#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
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
    struct sockaddr_in server;
    char buffer[BUFF_SIZE];
    int a = 2, b = 5;
    
    // Initialize network
    if (!initialize_network()) {
        return 1;
    }
    
    // Create socket and connect
    socket_t sock = create_and_connect("127.0.0.1", PORT, &server);
    #ifdef _WIN32
        if (sock == INVALID_SOCKET) {
            cleanup_network(sock);
            return 1;
        }
    #else
        if (sock < 0) {
            cleanup_network(sock);
            return 1;
        }
    #endif
    
    printf("Pripojeny k serveru na porte %d\n", PORT);
    printf("Odosielam cisla %d a %d\n", a, b);
    
    // Send data
    sprintf(buffer, "%d %d", a, b);
    send(sock, buffer, strlen(buffer), 0);
    
    // Receive response
    memset(buffer, 0, BUFF_SIZE);
    recv(sock, buffer, BUFF_SIZE, 0);
    printf("Odpoved zo servera: %s\n", buffer);
    
    // Cleanup
    cleanup_network(sock);
    
    return 0;
}