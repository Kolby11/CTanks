#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "shared/models/client.h"
#include "shared/serialization/message.h"
#include "shared/serialization/player_id.h"

#define BUFF_SIZE 1024

void* client_connection_thread(void *arg) {
    Client *client = (Client*)arg;
    char buffer[BUFF_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client->sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            Message msg;
            printf("Client %d received: %s\n", client->player.id, buffer);
            deserialize_message(buffer, &msg);
            printf("Message action: %u, Message data: %s", msg.action, msg.data);
        }
    }

    close(client->sock);

    free(client);
    return NULL;
}