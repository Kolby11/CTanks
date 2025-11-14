#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "connection/socket.h"
#include "shared/serialization/movement.h"
#include "shared/serialization/player_id.h"
#include "client/connection/client.h"
#include "shared/models/client.h"

#define PORT 5432
#define BUFF_SIZE 1024

int main(void) {
    struct sockaddr_in server;

    // Connect to server
    int sock = connect_server("127.0.0.1", PORT, &server);
    if (sock < 0) {
        printf("Failed to connect\n");
        return 1;
    }

    printf("Connected to server on port %d\n", PORT);

    Client *client = malloc(sizeof(Client));
    client->sock = sock;
    // init player id to something known (0 for now)
    client->player.id = 0;

    pthread_t tid;
    int err = pthread_create(&tid, NULL, connection_thread, client);
    if (err != 0) {
        printf("Failed to create thread\n");
        close(sock);
        free(client);
        return 1;
    }

    pthread_join(tid, NULL);

    printf("Main exiting\n");
    return 0;
}
