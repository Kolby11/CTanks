#ifndef SHARED_SERIALIZATION_ACTION_H
#define SHARED_SERIALIZATION_ACTION_H

#include <stdio.h>

int serialize_action(char *action, char *data, char *buffer, size_t buffer_size);
int deserialize_action(const char *buffer);

#endif