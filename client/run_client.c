#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "client/connection/socket.h"
#include "client/game_menu/game_menu.h"
#include "shared/models/client.h"
#include "input/input_handler.h"

#define BUFF_SIZE 1024

// Global client pointer for signal handler
Client *g_client = NULL;

// Signal handler for clean exit
void handle_signal(int sig) {
    if (g_client != NULL) {
        restore_input();
        close(g_client->sock);
        free(g_client);
    }
    printf("\nClient shutting down...\n");
    exit(0);
}

int run_client(char *ipv4_addr, int port) {
    struct sockaddr_in server;
    int sock = connect_server(ipv4_addr, port, &server);
    
    if (sock < 0) {
        printf("Failed to connect to server\n");
        return 1;
    }
    
    printf("Connected to server at %s:%d\n", ipv4_addr, port);
    
    Client *client = malloc(sizeof(Client));
    client->sock = sock;
    g_client = client;
    
    // Setup signal handlers for clean exit
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    // Initialize input handling
    init_input();
    printf("Use WASD for movement, Q to quit.\n");
    
    int running = 1;
    
    while(running) {
        client_receive_message(client);
        
        // Update game data using server message
        // update_game_state(client);
        
        // Draw/render game data
        // render_game(client);
        
        // Handle input and send to server
        handle_input(client);
        
        usleep(16000);
    }
    
    // Cleanup
    restore_input();
    close(client->sock);
    free(client);
    
    return 0;
}
