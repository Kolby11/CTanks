#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "shared/models/client.h"

void init_input();
void restore_input();
int kbhit();
char read_key();
void handle_input(Client *client);

#endif