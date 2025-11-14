#include <stdio.h>
#include "shared/serialization/player_id.h"

void serialize_player_id(PlayerId player_id, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size,
        "{\"player_id\": %d}",
        player_id);
}

int deserialize_player_id(const char *buffer, PlayerId *out) {
    return sscanf(buffer,
        "{\"player_id\": %d}",
        out);
}
