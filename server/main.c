#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "connection/socket.h"
#include "shared/serialization/player_id.h"

#define PORT 5432
#define BUFF_SIZE 1024
#define MAX_PLAYERS 4

typedef struct {
    int sock;
    int player_id;
} ClientContext;

pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;
int player_count = 0;

void* client_thread(void *arg) {
    ClientContext *ctx = (ClientContext*)arg;
    char buffer[BUFF_SIZE];

    printf("Client thread started for player %d\n", ctx->player_id);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(ctx->sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            printf("Player %d disconnected\n", ctx->player_id);
            break;
        }

        printf("Player %d says: %s\n", ctx->player_id, buffer);
        send(ctx->sock, buffer, bytes, 0);
    }

    close(ctx->sock);

    // Optionally decrement player_count with mutex, mark slot free, etc.
    free(ctx);
    return NULL;
}

int main(void) {
    struct sockaddr_in server, client;

    int server_sock = create_server_socket(PORT, &server);
    if (server_sock < 0) {
        printf("Failed to create server socket\n");
        cleanup_network(server_sock);
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

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
        int this_id = player_count++;
        pthread_mutex_unlock(&players_mutex);

        char id_buf[64];
        serialize_player_id(this_id, id_buf, sizeof(id_buf));
        send(client_sock, id_buf, strlen(id_buf), 0);

        ClientContext *ctx = malloc(sizeof(ClientContext));
        ctx->sock = client_sock;
        ctx->player_id = this_id;

        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, ctx);
        pthread_detach(tid); // no need to join
    }

    close(server_sock);
    return 0;
}
