#include <stdio.h>
#include "movement.h"

void player_move(Client *client, Vector2 move_direction){
    int result = send_message(client, PLAYER_MOVE, &move_direction, sizeof(Vector2));
}