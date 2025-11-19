#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "connection/socket.h"
#include "shared/serialization/player_id.h"
#include "shared/models/client.h"
#include "server/connection/client.h"

pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;

int run_server(const int PORT, const int MAX_PLAYERS) {
    int player_count = 0;
    Client clients[MAX_PLAYERS];
    struct sockaddr_in server, client;

    int server_sock = create_server_socket(PORT, &server);
    if (server_sock < 0) {
        printf("Failed to main create server socket\n");
        cleanup_network(server_sock);
        return 1;
    }

    printf("Main server listening on port %d\n", PORT);

    while (1) {
        socklen_t client_len = sizeof(client);
        int client_sock = accept_client(server_sock, &client);
        if (client_sock < 0) {
            printf("Accept failed\n");
            continue;
        }

        pthread_mutex_lock(&players_mutex);

        if (player_count >= MAX_PLAYERS) {
            pthread_mutex_unlock(&players_mutex);
            const char *msg = "{\"error\": \"server full\"}";
            send(client_sock, msg, strlen(msg), 0);
            close(client_sock);
            continue;
        }

        Client *client = malloc(sizeof(Client));
        client->sock = client_sock;
        client->player.id = player_count++;

        pthread_mutex_unlock(&players_mutex);

        char id_buf[64];
        serialize_player_id(client->player.id, id_buf, sizeof(id_buf));
        send(client_sock, id_buf, strlen(id_buf), 0);

        pthread_t tid;
        pthread_create(&tid, NULL, server_connection_thread, client);
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}
