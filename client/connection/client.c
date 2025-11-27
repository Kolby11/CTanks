#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "shared/models/client.h"
#include "shared/connection/socket.h"

void* client_connection_thread(void *arg) {
    Client *client = (Client*)arg;

    while (1) {
        receive_message(client);
    }

    close(client->sock);

    free(client);
    return NULL;
}