#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #define close closesocket
    typedef SOCKET socket_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int socket_t;
#endif

// Function declarations
int initialize_network();
socket_t create_server_socket(int port, struct sockaddr_in *server);
socket_t accept_client(socket_t serverSock, struct sockaddr_in *client);
void cleanup_network(socket_t sock);

// Platform-specific implementations
#ifdef _WIN32

int initialize_network() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 0;
    }
    return 1;
}

socket_t create_server_socket(int port, struct sockaddr_in *server) {
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return INVALID_SOCKET;
    }
    
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = INADDR_ANY;
    server->sin_port = htons(port);
    
    if (bind(serverSock, (struct sockaddr*)server, sizeof(*server)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(serverSock);
        return INVALID_SOCKET;
    }
    
    if (listen(serverSock, 3) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(serverSock);
        return INVALID_SOCKET;
    }
    
    return serverSock;
}

socket_t accept_client(socket_t serverSock, struct sockaddr_in *client) {
    int clientLen = sizeof(*client);
    SOCKET clientSock = accept(serverSock, (struct sockaddr*)client, &clientLen);
    if (clientSock == INVALID_SOCKET) {
        printf("Accept failed\n");
        return INVALID_SOCKET;
    }
    return clientSock;
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

socket_t create_server_socket(int port, struct sockaddr_in *server) {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        printf("Socket creation failed\n");
        return -1;
    }
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = INADDR_ANY;
    server->sin_port = htons(port);
    
    if (bind(serverSock, (struct sockaddr*)server, sizeof(*server)) < 0) {
        printf("Bind failed\n");
        close(serverSock);
        return -1;
    }
    
    if (listen(serverSock, 3) < 0) {
        printf("Listen failed\n");
        close(serverSock);
        return -1;
    }
    
    return serverSock;
}

socket_t accept_client(socket_t serverSock, struct sockaddr_in *client) {
    socklen_t clientLen = sizeof(*client);
    int clientSock = accept(serverSock, (struct sockaddr*)client, &clientLen);
    if (clientSock < 0) {
        printf("Accept failed\n");
        return -1;
    }
    return clientSock;
}

void cleanup_network(socket_t sock) {
    if (sock >= 0) {
        close(sock);
    }
}

#endif