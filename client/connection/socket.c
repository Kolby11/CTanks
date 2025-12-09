#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <shared/models/client.h>
#include <shared/models/message.h>
#include <shared/models/movement.h>
#include "socket.h"

#define BUFF_SIZE 512

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

int client_receive_message(Client *client) {
    char buffer[BUFF_SIZE];
    memset(buffer, 0, sizeof(buffer));

    MessageType message_type;
    ssize_t bytes_received = recv(client->sock, &message_type, sizeof(MessageType), MSG_DONTWAIT);

    if (bytes_received <= 0) {
        return 0;
    }

    if (message_type == NONE) {
        return 0;
    }

    switch (message_type) {
        case SERVER_STATUS:
            printf("Server full\n");
            break;
        case PLAYER_ASSIGN_ID: {
            PlayerId player_id;
            bytes_received = recv(client->sock, &player_id, sizeof(PlayerId), MSG_DONTWAIT);
            if (bytes_received > 0) {
                client->player.id = player_id;
                printf("Player id: %d\n", client->player.id);
            }
            break;
        }
        case PLAYER_JOIN: {
            PlayerId player_id;
            bytes_received = recv(client->sock, &player_id, sizeof(PlayerId), MSG_DONTWAIT);
            if (bytes_received > 0) {
                printf("Player %d connected\n", player_id);
            }
            break;
        }
        case PLAYER_LEAVE: {
            PlayerId player_id;
            bytes_received = recv(client->sock, &player_id, sizeof(PlayerId), MSG_DONTWAIT);
            if (bytes_received > 0) {
                printf("Player %d disconnected\n", player_id);
            }
            break;
        }
        case PLAYER_MOVE: {
            MoveData move_data;
            bytes_received = recv(client->sock, &move_data, sizeof(MoveData), MSG_DONTWAIT);
            if (bytes_received > 0) {
                printf("Player %d moved: %d %d\n", move_data.player_id, move_data.direction.x, move_data.direction.y);
            }
            break;
        }
        case PLAYER_ATTACK:
            printf("Player %d attacked\n", client->player.id);
            break;
        default:
            printf("Received unknown message type: %d\n", message_type);
            break;
    }

    return 0;
}