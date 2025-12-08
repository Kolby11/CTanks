#include <stdio.h>

#include "shared/models/client.h"
#include "shared/models/message.h"
#include "shared/models/player.h"
#include "shared/models/movement.h"
#include <unistd.h>

#define BUFF_SIZE 512

int send_message(Client *client, MessageType message_type, void *data, size_t data_size) {
    send(client->sock, &message_type, sizeof(MessageType), 0);
    send(client->sock, data, data_size, 0);
    return 0;
}