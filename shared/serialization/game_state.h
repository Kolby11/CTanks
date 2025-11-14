#ifndef SHARED_SERIALIZATION_GAME_STATE_H
#define SHARED_SERIALIZATION_GAME_STATE_H

#include <stdio.h>
#include "shared/data/game_state.h"

void serialize_game_state(GameState game_state, char *buffer, size_t buffer_size);
GameState deserialize_game_state(const char *buffer);

#endif
