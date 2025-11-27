#include <stdlib.h>
#include <netinet/in.h>

#include "shared/models/client.h"
#include "shared/connection/socket.h"

#define BUFF_SIZE 1024

void* server_connection_thread(void *arg) {
    Client *client = (Client*)arg;
    char buffer[BUFF_SIZE];

    while (1) {
        receive_message(client);
    }

    close(client->sock);

    free(client);
    return NULL;
}