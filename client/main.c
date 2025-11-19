#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "shared/serialization/movement.h"
#include "shared/serialization/player_id.h"
#include "client/game_menu/game_menu.h"
#include "server/connection/client.h"
#include "shared/models/client.h"

#define BUFF_SIZE 1024

int main(void) {
    main_menu();

    // Client *client = malloc(sizeof(Client));
    // client->sock = sock;
    // // init player id to something known (0 for now)
    // client->player.id = 0;

    // pthread_t tid;
    // int err = pthread_create(&tid, NULL, client_connection_thread, client);
    // if (err != 0) {
    //     printf("Failed to create thread\n");
    //     close(sock);
    //     free(client);
    //     return 1;
    // }

    // pthread_join(tid, NULL);

    printf("Main exiting\n");
    return 0;
}
