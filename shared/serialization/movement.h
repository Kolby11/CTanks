#ifndef SHARED_SERIALIZATION_MOVEMENT_H
#define SHARED_SERIALIZATION_MOVEMENT_H

#include <stdio.h>

void serialize_movement(int player, int x, int y, int z, char *buffer, size_t buffer_size);
int deserialize_movement(const char *buffer, int *player, int *x, int *y, int *z);

#endif