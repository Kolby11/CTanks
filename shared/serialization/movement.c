#include <stdio.h>
#include <string.h>
#include "movement.h"

void serialize_movement_data(const PlayerMovementData *data, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size,
        "{\"player\": %d, \"move\": \"(%d %d)\"}",
        data->player, data->x, data->y);
}

int deserialize_movement(const char *buffer, const PlayerMovementData *out) {
    return sscanf(buffer,
        "{\"player\": %d, \"move\": \"(%d %d)\"}",
        out->player, out->x, out->y);
}