#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "connection/socket.h"
#include "shared/serialization/movement.h"
#include "shared/serialization/player_id.h"

#define PORT 5432
#define BUFF_SIZE 1024

PlayerId player_id = 0;

int main() {
    struct sockaddr_in server;
    char buffer[BUFF_SIZE];
    int player_id = 1;

    // Connect to server
    int sock = connect_server("127.0.0.1", PORT, &server);
    if (sock < 0) {
        printf("Failed to connect\n");
        return 1;
    }

    printf("Connected to server on port %d\n", PORT);

    // serialize_movement(player_id, 1, 0, buffer, BUFF_SIZE);
    send(sock, buffer, strlen(buffer), 0);

    // Receive response
    memset(buffer, 0, BUFF_SIZE);
    recv(sock, buffer, BUFF_SIZE, 0);

    deserialize_player_id(buffer, &player_id);
    printf("Response from server: %d\n", player_id);

    // Cleanup
    cleanup_socket(sock);

    return 0;
}
