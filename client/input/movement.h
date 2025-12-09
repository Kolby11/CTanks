#ifndef CLIENT_INPUT_MOVEMENT_H
#define CLIENT_INPUT_MOVEMENT_H

#include "shared/connection/socket.h"
#include "shared/models/movement.h"

void player_move(Client *client, Vector2 move_direction);

#endif