#include "input_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <string.h>

#include "shared/models/message.h"
#include "shared/models/movement.h"
#include "client/movement/movement.h"

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

void handle_input(Client *client) {
    if (kbhit()) {
        char key = read_key();
        
        if (key != 0) {
            // printf("Key pressed: %c\n", key);
            
            switch(key) {
                case 'w':
                case 'W':
                    printf("Moving UP\n");
                    player_move(client, UP);
                    break;
                case 's':
                case 'S':
                    printf("Moving DOWN\n");
                    player_move(client, DOWN);
                    break;
                case 'a':
                case 'A':
                    printf("Moving LEFT\n");
                    player_move(client, LEFT);
                    break;
                case 'd':
                case 'D':
                    printf("Moving RIGHT\n");
                    player_move(client, RIGHT);
                    break;
                case 'q':
                case 'Q':
                    printf("Quitting\n");
                    restore_input();
                    exit(0);
                    break;
                default:
                    printf("Unknown key: %c (ASCII: %d)\n", key, (int)key);
                    break;
            }
        }
    }
}