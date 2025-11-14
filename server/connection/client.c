#include "shared/serialization/player_id.h"
#include "shared/models/client.h"

#define BUFF_SIZE 1024

void* connection_thread(void *arg) {
    Client *client = (Client*)arg;
    char buffer[BUFF_SIZE];

    printf("Client thread started for client %d\n", client->player.id);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client->sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            printf("Client %d says: %s\n", client->player.id, buffer);
        }

    }

    close(client->sock);

    free(client);
    return NULL;
}