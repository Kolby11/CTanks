#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <netinet/in.h>
#include <shared/models/client.h>

int connect_server(const char *ip_addr, int port, struct sockaddr_in *server);
void cleanup_socket(int sock);
int client_receive_message(Client *client);

#endif