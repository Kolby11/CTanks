#ifndef CLIENT_INPUT_HANDLER_H
#define CLIENT_INPUT_HANDLER_H

#include "shared/models/client.h"

void init_input();
void restore_input();
int kbhit();
char read_key();
int handle_input(Client *client, int *running);

#endif