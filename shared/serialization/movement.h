#ifndef SHARED_SERIALIZATION_MOVEMENT_H
#define SHARED_SERIALIZATION_MOVEMENT_H

#include <stdio.h>

typedef struct PlayerMovementData {
    int player;
    int x;
    int y;
} PlayerMovementData;

void serialize_movement(const PlayerMovementData *data, char *buffer, size_t buffer_size);
int deserialize_movement(const char *buffer, const PlayerMovementData *out);

#endif
