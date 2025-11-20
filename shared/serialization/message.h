#ifndef SHARED_SERIALIZATION_ACTION_H
#define SHARED_SERIALIZATION_ACTION_H

#include <stdio.h>
#include "shared/models/message.h"

int serialize_message(Message *message, char *buffer, size_t buffer_size);
int deserialize_message(const char *buffer, Message *message);

#endif