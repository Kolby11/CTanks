#include <stdio.h>

#include "shared/models/client.h"
#include "shared/models/message.h"
#include "shared/models/player.h"

#define BUFF_SIZE 512

int receive_message(Client *client) {
    char buffer[BUFF_SIZE];
    memset(buffer, 0, sizeof(buffer));

    MessageType message_type = recv(client->sock, buffer, sizeof(buffer) - 1, 0);
    
    switch (message_type) {
        case SERVER_FULL:
            printf("Server full");
            break;
        case PLAYER_ASSIGNED_ID:
            PlayerId player_id = recv(client->sock, buffer, sizeof(buffer) - 1, 0);
            printf("Received player id: %d", player_id);
            break;
        case PLAYER_JOINED:
            /* code */
            break;
        case PLAYER_LEFT:
            /* code */
            break;
        
        default:
            printf("Received unknown message type");
            break;
    }
}

int send_message(Client *client, MessageType message_type, void *data, size_t data_size) {
    send(client->sock, message_type, sizeof(MessageType), 0);
    send(client->sock, data, data_size, 0);
}