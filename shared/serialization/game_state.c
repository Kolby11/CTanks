#include <stdio.h>
#include "../data/game_state.h"

void serialize_game_state(GameState game_state, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "{'game_state': '%d'}", game_state);
}

GameState deserialize_game_state(const char *buffer) {
    return sscanf(buffer, "{'game_state': '%d'}", game_state);
    
}