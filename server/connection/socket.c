#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int create_server_socket(int port, struct sockaddr_in *server) {
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


int accept_client(int serverSock, struct sockaddr_in *client) {
    socklen_t clientLen = sizeof(*client);
    int clientSock = accept(serverSock, (struct sockaddr*)client, &clientLen);
    if (clientSock < 0) {
        printf("Accept failed\n");
        return -1;
    }
    return clientSock;
}

void cleanup_network(int sock) {
    if (sock >= 0) {
        close(sock);
    }
}