#ifndef SHARED_SERIALIZATION_PLAYER_ID_H
#define SHARED_SERIALIZATION_PLAYER_ID_H

#include <stdio.h>
#include "shared/data/player_id.h"

void serialize_player_id(PlayerId player_id, char *buffer, size_t buffer_size);
int deserialize_player_id(const char *buffer, PlayerId *out);

#endif
