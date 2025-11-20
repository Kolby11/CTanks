#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client/connection/socket.h"
#include "client/connection/client.h"
#include "client/game_menu/game_menu.h"
#include "shared/models/client.h"

#define BUFF_SIZE 1024

int run_client(char *ipv4_addr, int port) {
    struct sockaddr_in server;
    int sock = connect_server(ipv4_addr, port, &server);

    if (sock < 0) {
        printf("Failed to connect to server\n");
        return 1;
    }
    
    Client *client = malloc(sizeof(Client));
    client->sock = sock;

    pthread_t tid;
    pthread_create(&tid, NULL, client_connection_thread, client);
    pthread_detach(tid);

    for(int i = 0; i < 50; i++) {
        sleep(1);
        i++;
    }
}
