#include <stdlib.h>
#include <netinet/in.h>

#include "shared/models/client.h"
#include "shared/connection/socket.h"

void* server_connection_thread(void *arg) {
    Client *client = (Client*)arg;

    while (1) {
        int result = server_receive_message(client);
        if (result < 0) {
            // Client disconnected or error occurred
            break;
        }
    }

    close(client->sock);
    free(client);
    return NULL;
}