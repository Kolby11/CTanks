#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include "connection/socket.h"
#include "shared/models/client.h"
#include "shared/connection/socket.h"

pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;

int run_server(const int PORT, const int MAX_PLAYERS, int *PIPE_FD) {
    close(PIPE_FD[0]);
    int client_count = 0;
    Client clients[MAX_PLAYERS];
    struct sockaddr_in server, client;
    
    int server_sock = create_server_socket(PORT, &server);
    if (server_sock < 0) {
        printf("Failed to main create server socket\n");
        cleanup_network(server_sock);
        return 1;
    }

    printf("Server listening on port %d\n", PORT);
    write(PIPE_FD[1], &PORT, 1);
    close(PIPE_FD[1]);

    while (1) {
        int client_sock = accept_client(server_sock, &client);
        Client *client = malloc(sizeof(Client));
        client->sock = client_sock;

        if (client_sock < 0) {
            printf("Accept failed\n");
            continue;
        }
        
        pthread_mutex_lock(&players_mutex);
        
        if (client_count >= MAX_PLAYERS) {
            pthread_mutex_unlock(&players_mutex);
            int data = 1;
            send_message(client, SERVER_STATUS, &data, sizeof(int));
            close(client_sock);
            continue;
        }
        
        client->player.id = client_count;
        clients[client_count] = *client;
        client_count++;
        pthread_mutex_unlock(&players_mutex);

        send_message(client, PLAYER_ASSIGN_ID, &client->player.id, sizeof(PlayerId));

        ClientThreadContext *ctx = malloc(sizeof(ClientThreadContext));
        ctx->client = client;
        ctx->all_clients = clients;
        ctx->client_count = &client_count;
        ctx->max_players = MAX_PLAYERS;
        ctx->mutex = &players_mutex;

        pthread_t tid;
        pthread_create(&tid, NULL, server_connection_thread, ctx);
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}
