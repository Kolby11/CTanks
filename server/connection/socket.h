#ifndef SERVER_CONNECTION_SOCKET_H
#define SERVER_CONNECTION_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "shared/models/client.h"
#include "shared/models/client.h"
#include "shared/connection/socket.h"

typedef struct {
    Client *client;
    Client *all_clients;
    int *client_count;
    int max_players;
    pthread_mutex_t *mutex;
} ClientThreadContext;

int create_server_socket(int port, struct sockaddr_in *server);
int accept_client(int serverSock, struct sockaddr_in *client);
void cleanup_network(int sock);
int server_receive_message(Client *client, MessageType *out_message_type, void *out_data, size_t *out_data_size);
void* server_connection_thread(void *arg);

#endif