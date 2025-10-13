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

int initialize_network();
socket_t create_and_connect(const char *ip_addr, int port, struct sockaddr_in *server);
void cleanup_network(socket_t sock);

#ifdef _WIN32
int initialize_network() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 0;
    }
    return 1;
}

socket_t create_and_connect(PCSTR ip_addr, int port, struct sockaddr_in *server) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return INVALID_SOCKET;
    }
    
    server->sin_family = AF_INET;
    server->sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server->sin_addr);
    
    if (connect(sock, (struct sockaddr*)server, sizeof(*server)) < 0) {
        printf("Connection failed\n");
        closesocket(sock);
        return INVALID_SOCKET;
    }
    
    return sock;
}

void cleanup_network(socket_t sock) {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    WSACleanup();
}

#else  // Linux

int initialize_network() {
    return 1;
}

socket_t create_and_connect(const char *ip_addr, int port, struct sockaddr_in *server) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation failed\n");
        return -1;
    }
    
    server->sin_family = AF_INET;
    server->sin_port = htons(port);
    inet_pton(AF_INET, ip_addr, &server->sin_addr);
    
    if (connect(sock, (struct sockaddr*)server, sizeof(*server)) < 0) {
        printf("Connection failed\n");
        close(sock);
        return -1;
    }
    
    return sock;
}

void cleanup_network(socket_t sock) {
    if (sock >= 0) {
        close(sock);
    }
}

#endif