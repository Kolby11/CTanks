#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

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

// Function declarations
int initialize_network();
socket_t create_server_socket(int port, struct sockaddr_in *server);
socket_t accept_client(socket_t serverSock, struct sockaddr_in *client);
void cleanup_network(socket_t sock);

#endif