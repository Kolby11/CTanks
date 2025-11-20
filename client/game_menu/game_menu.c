#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "client/connection/socket.h"
#include "client/run_client.h"
#include "client/game_menu/dialogs.h"

#include "server/run_server.h"

#include "shared/validation/network.h"

#define MAX_PLAYERS 4

int main_menu(void) {
    char ipv4_addr[16] = "127.0.0.1\0";
    int port = 5432;
    pid_t pid = -1;

    int option = ask_for_host_or_join();

    if (option == 1) {
        ask_for_port(&port);
        pid = fork();
        if (pid < 0) {
            printf("Creating client process failed\n");
            return 1;
        }

        if (pid > 0) {
            run_server(port, MAX_PLAYERS);
            return 0;
        }
    } else if (option == 2) {
        ask_for_ip(ipv4_addr, sizeof(ipv4_addr));
        ask_for_port(&port);
    } else {
        printf("Invalid option\n");
        return 1;
    }

    if (option == 2 || pid == 0) {
        run_client(ipv4_addr, port);
    }

    return 0;
}