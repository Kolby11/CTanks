#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include "client/connection/socket.h"
#include "client/game_menu/game_menu.h"
#include "shared/models/client.h"
#include "input/input_handler.h"

#define BUFF_SIZE 1024
#define IDLE_TIMEOUT_SEC 15

Client *g_client = NULL;
volatile sig_atomic_t g_idle_timeout = 0;

void handle_idle_timeout(int sig) {
    (void)sig;
    g_idle_timeout = 1;
}

void handle_signal(int sig) {
    if (g_client != NULL) {
        restore_input();
        close(g_client->sock);
        free(g_client);
    }
    printf("\nClient shutting down...\n");
    exit(0);
}

timer_t create_timer(int signal) {
    struct sigevent kam;
    kam.sigev_notify = SIGEV_SIGNAL;
    kam.sigev_signo = signal;

    timer_t timer;
    timer_create(CLOCK_REALTIME, &kam, &timer);
    return timer;
}

void start_timer(timer_t timer, int seconds) {
    struct itimerspec time;
    time.it_value.tv_sec = seconds;
    time.it_value.tv_nsec = 0;
    time.it_interval.tv_sec = 0;
    time.it_interval.tv_nsec = 0;

    timer_settime(timer, 0, &time, NULL);
}

int run_client(char *ipv4_addr, int port) {
    struct sockaddr_in server;
    GameCtx game_ctx;

    int sock = connect_server(ipv4_addr, port, &server);
    
    if (sock < 0) {
        printf("Failed to connect to server\n");
        return 1;
    }
    
    printf("Connected to server at %s:%d\n", ipv4_addr, port);
    
    Client *client = malloc(sizeof(Client));
    client->sock = sock;
    g_client = client;
    
    // Setup signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGUSR1, handle_idle_timeout);

    // Initialize input handling
    init_input();
    printf("Use WASD for movement, Q to quit.\n");

    // Create and start idle timer
    timer_t idle_timer = create_timer(SIGUSR1);
    start_timer(idle_timer, IDLE_TIMEOUT_SEC);

    int running = 1;

    while(running && !g_idle_timeout) {
        client_receive_message(client);

        // Update game data using server message

        // Draw/render game data

        int input_received = handle_input(client, &running);

        if (input_received) {
            start_timer(idle_timer, IDLE_TIMEOUT_SEC);
        }

        usleep(16000);
    }

    if (g_idle_timeout) {
        printf("\nDisconnected due to %d seconds of inactivity\n", IDLE_TIMEOUT_SEC);
    }

    timer_delete(idle_timer);

    if (client->sock >= 0) {
        shutdown(client->sock, SHUT_RDWR);
        close(client->sock);
    }

    restore_input();
    free(client);

    return 0;
}
