#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <shared/models/client.h>
#include <shared/models/message.h>
#include <shared/models/movement.h>

#include "server/connection/socket.h"
#include "shared/connection/socket.h"

#define BUFF_SIZE 512

int create_server_socket(int port, struct sockaddr_in *server) {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        printf("Socket creation failed\n");
        return -1;
    }
    
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

int server_receive_message(Client *client, MessageType *out_message_type, void *out_data, size_t *out_data_size) {
    char buffer[BUFF_SIZE];
    memset(buffer, 0, sizeof(buffer));

    MessageType message_type;
    ssize_t bytes_received = recv(client->sock, &message_type, sizeof(MessageType), 0);

    if (bytes_received <= 0) {
        return -1;
    }

    if (message_type == NONE) {
        return 0;
    }

    *out_message_type = message_type;

    switch (message_type) {
        case PLAYER_MOVE: {
            MoveData move_data;
            move_data.player_id = client->player.id;
            bytes_received = recv(client->sock, &move_data.direction, sizeof(Vector2), 0);
            if (bytes_received > 0) {
                memcpy(out_data, &move_data, sizeof(MoveData));
                *out_data_size = sizeof(MoveData);
            } else {
                *out_data_size = 0;
            }
            break;
        }
        case PLAYER_ATTACK: {
            *out_data_size = 0;
            break;
        }
        default:
            printf("Server received unexpected message type: %d from player %d\n", message_type, client->player.id);
            *out_data_size = 0;
            break;
    }

    return 0;
}

void* server_connection_thread(void *arg) {
    ClientThreadContext *ctx = (ClientThreadContext*)arg;
    Client *client = ctx->client;
    char data_buffer[BUFF_SIZE];
    MessageType message_type;
    size_t data_size = sizeof(ctx->client->player.id);

    // printf("Player %d connected\n", client->player.id);

    broadcast_message(ctx->all_clients, *ctx->client_count, PLAYER_JOIN,
                     &ctx->client->player.id, data_size);

    while (1) {
        int result = server_receive_message(client, &message_type, data_buffer, &data_size);

        if (result < 0) {
            // printf("Player %d disconnected\n", client->player.id);

            pthread_mutex_lock(ctx->mutex);
            for (int i = 0; i < *ctx->client_count; i++) {
                if (ctx->all_clients[i].player.id == client->player.id) {
                    ctx->all_clients[i].sock = -1;
                    break;
                }
            }

            broadcast_message(ctx->all_clients, *ctx->client_count, PLAYER_LEAVE,
                            &ctx->client->player.id, sizeof(ctx->client->player.id));
            pthread_mutex_unlock(ctx->mutex);
            break;
        }

        // Broadcast to all clients
        if (message_type != NONE && data_size > 0) {
            pthread_mutex_lock(ctx->mutex);
            broadcast_message(ctx->all_clients, *ctx->client_count, message_type,
                            data_buffer, data_size);
            pthread_mutex_unlock(ctx->mutex);
        }
    }

    // Cleanup
    close(client->sock);
    free(client);
    free(ctx);
    return NULL;
}