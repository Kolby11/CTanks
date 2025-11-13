#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int connect_server(const char *ip_addr, int port, struct sockaddr_in *server);
void cleanup_socket(int sock);

int connect_server(const char *ip_addr, int port, struct sockaddr_in *server) {
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

void cleanup_socket(int sock) {
    if (sock >= 0) {
        close(sock);
    }
}