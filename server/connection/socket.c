#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <shared/models/client.h>
#include <shared/models/message.h>
#include <shared/models/movement.h>

#define BUFF_SIZE 512

int create_server_socket(int port, struct sockaddr_in *server) {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        printf("Socket creation failed\n");
        return -1;
    }
    
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = INADDR_ANY;
    server->sin_port = htons(port);
    
    if (bind(serverSock, (struct sockaddr*)server, sizeof(*server)) < 0) {
        printf("Bind failed\n");
        close(serverSock);
        return -1;
    }
    
    if (listen(serverSock, 3) < 0) {
        printf("Listen failed\n");
        close(serverSock);
        return -1;
    }
    
    return serverSock;
}


int accept_client(int serverSock, struct sockaddr_in *client) {
    socklen_t clientLen = sizeof(*client);
    int clientSock = accept(serverSock, (struct sockaddr*)client, &clientLen);
    if (clientSock < 0) {
        printf("Accept failed\n");
        return -1;
    }
    return clientSock;
}

void cleanup_network(int sock) {
    if (sock >= 0) {
        close(sock);
    }
}

int server_receive_message(Client *client) {
    char buffer[BUFF_SIZE];
    memset(buffer, 0, sizeof(buffer));

    MessageType message_type;
    ssize_t bytes_received = recv(client->sock, &message_type, sizeof(MessageType), 0);

    if (bytes_received <= 0) {
        printf("Client %d disconnected\n", client->player.id);
        return -1;
    }

    if (message_type == NONE) {
        return 0;
    }

    switch (message_type) {
        case SERVER_STATUS:
            printf("Server full\n");
            break;
        case PLAYER_JOIN:
            printf("Player %d joined\n", client->player.id);
            break;
        case PLAYER_MOVE: {
            MoveDirection move_direction;
            bytes_received = recv(client->sock, &move_direction, sizeof(MoveDirection), 0);
            if (bytes_received > 0) {
                printf("Player %d moved: %d\n", client->player.id, move_direction);
                // Echo back the movement to the client
                send_message(client, PLAYER_MOVE, &move_direction, sizeof(MoveDirection));
            }
            break;
        }
        case PLAYER_LEAVE:
            printf("Player %d left\n", client->player.id);
            return -1;
        default:
            break;
    }

    return 0;
}
