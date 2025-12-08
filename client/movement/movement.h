#ifndef CLIENT_MOVEMENT_H
#define CLIENT_MOVEMENT_H

#include "shared/connection/socket.h"
#include "shared/models/message.h"
#include "shared/models/movement.h"

void player_move(Client *client, MoveDirection move_direction);

#endif