#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>

#include "shared/models/client.h"
#include "shared/models/message.h"
#include "shared/models/player.h"
#include "shared/models/movement.h"
#include <unistd.h>

#define BUFF_SIZE 512


int send_message(Client *client, MessageType message_type, void *data, size_t data_size) {
    // Check if socket is valid before sending
    if (client->sock < 0) {
        return -1;
    }

    // Send message type
    ssize_t result = send(client->sock, &message_type, sizeof(MessageType), 0);
    if (result < 0) {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN) {
            // Connection closed, mark socket as invalid
            client->sock = -1;
        }
        return -1;
    }

    // Send data if there is any
    if (data != NULL && data_size > 0) {
        result = send(client->sock, data, data_size, 0);
        if (result < 0) {
            if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN) {
                client->sock = -1;
            }
            return -1;
        }
    }

    return 0;
}

int broadcast_message(Client *clients, int client_count, MessageType message_type, void *data, size_t data_size) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sock >= 0) {
            send_message(&clients[i], message_type, data, data_size);
        }
    }
    return 0;
}