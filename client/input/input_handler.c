#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <string.h>

#include "input_handler.h"
#include "movement.h"
#include "attack.h"
#include "shared/models/message.h"
#include "shared/models/movement.h"

static struct termios old_term, new_term;

void init_input() {
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    
    new_term.c_lflag &= ~(ICANON | ECHO);
    
    new_term.c_cc[VMIN] = 0;
    new_term.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void restore_input() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}

int kbhit() {
    struct timeval tv = {0, 0};
    fd_set readfds;
    
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

char read_key() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        return c;
    }
    return 0;
}

int handle_input(Client *client, int *running) {
    if (kbhit()) {
        char key = read_key();

        if (key != 0) {
            Vector2 move_vector = {0, 0};

            switch(key) {
                case 'w':
                case 'W':
                    move_vector.y += 1;
                    break;
                case 's':
                case 'S':
                    move_vector.y -= 1;
                    break;
                case 'a':
                case 'A':
                    move_vector.x -= 1;
                    break;
                case 'd':
                case 'D':
                    move_vector.x += 1;
                    break;
                case ' ':
                    player_attack(client);
                    return 1;  // Input received
                case 'q':
                case 'Q':
                    restore_input();
                    *running = 0;
                    return 1;  // Input received
                default:
                    printf("Unknown key: %c (ASCII: %d)\n", key, (int)key);
                    return 0;  // Unknown key doesn't reset timer
            }

            if (move_vector.x != 0 || move_vector.y != 0) {
                player_move(client, move_vector);
                return 1;  // Input received
            }
        }
    }
    return 0;  // No input
} 