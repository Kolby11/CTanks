#ifndef SHARED_DATA_CLIENT_H
#define SHARED_DATA_CLIENT_H

#include "player.h"

typedef struct Client {
    int sock;
    Player player;
} Client;

#endif

