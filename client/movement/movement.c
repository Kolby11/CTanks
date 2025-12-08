#include "movement.h"

void player_move(Client *client, MoveDirection move_direction){
    send_message(client, PLAYER_MOVE, &move_direction, sizeof(MoveDirection));
}