#include <stdlib.h>
#include <netinet/in.h>

#include "shared/serialization/player_id.h"
#include "shared/serialization/message.h"
#include "shared/models/client.h"

#define BUFF_SIZE 1024

void* server_connection_thread(void *arg) {
    Client *client = (Client*)arg;
    char buffer[BUFF_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client->sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            Message msg = {0};
            deserialize_message(buffer, &msg);
            printf("Client %d says: %s\n", client->player.id, buffer);
        }

        if (bytes <= 0) {
            printf("Client %d disconnected\n", client->player.id);
            break;
        }
    }

    close(client->sock);

    free(client);
    return NULL;
}