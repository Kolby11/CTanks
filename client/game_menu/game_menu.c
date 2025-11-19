#include "client/connection/socket.h"
#include "server/connection/socket.h"
#include <netinet/in.h>

void main_menu(){
    int option;
    printf("1. Host game\n");
    printf("2. Join game\n");
    printf("Select an option: ");
    if (scanf("%d", &option) != 1) {
        printf("Failed to read option\n");
        return 1;
    }

    if(option == 1){
        int port;
        printf("Enter port to host server on: ");
        scanf("%d", &port);
        struct sockaddr_in server;
        int server_sock = create_server_socket(port, &server);
        if (server_sock < 0) {
            printf("Failed to create server socket\n");
            return 1;
        }
    } else if(option == 2){
        char ipv4_addr[15];
        int port;
        printf("Enter server IPv4 address: ");
        scanf("%s", ipv4_addr);
        printf("Enter server port: ");
        scanf("%d", &port);
        struct sockaddr_in server;
        connect_server(ipv4_addr, port, &server);
    } else {
        printf("Invalid option\n");
    }
}