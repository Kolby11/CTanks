#ifndef SHARED_MODELS_MESSAGE_H
#define SHARED_MODELS_MESSAGE_H

#include "shared/models/game_state.h"

#define MAX_DATA_SIZE 512

typedef enum {
    NONE,
    SERVER_STATUS,
    GAME_STATE,
    PLAYER_ASSIGN_ID,
    PLAYER_JOIN,
    PLAYER_LEAVE,
    PLAYER_MOVE,
    PLAYER_ATTACK
} MessageType;

typedef GameState GameStateChangedData;

#endif
