#ifndef SERVER_CONNECTION_SOCKET_H
#define SERVER_CONNECTION_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "shared/models/client.h"

int create_server_socket(int port, struct sockaddr_in *server);
int accept_client(int serverSock, struct sockaddr_in *client);
void cleanup_network(int sock);
int server_receive_message(Client *client);

#endif