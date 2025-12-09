#include <stdlib.h>
#include "attack.h"

void player_attack(Client *client){
    send_message(client, PLAYER_ATTACK, NULL, NULL);
}