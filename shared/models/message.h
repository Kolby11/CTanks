#ifndef SHARED_MODELS_MESSAGE_H
#define SHARED_MODELS_MESSAGE_H

#include "shared/models/game_state.h"

#define MAX_DATA_SIZE 512

typedef enum {
    SERVER_FULL,
    PLAYER_ASSIGNED_ID,
    GAME_STATE_CHANGED,
    PLAYER_JOINED,
    PLAYER_LEFT,
    PLAYER_MOVED,
} MessageType;

typedef GameState GameStateChangedData;

#endif
