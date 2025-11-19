#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "shared/serialization/movement.h"
#include "shared/serialization/player_id.h"
#include "client/game_menu/game_menu.h"
#include "server/connection/client.h"
#include "shared/models/client.h"

#define BUFF_SIZE 1024

int run_client() {
    return 0;
}
