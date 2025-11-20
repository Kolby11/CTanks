#include <stdio.h>

#include "shared/models/message.h"

int serialize_message(Message *message, char *buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size,
        "{\"action\": %u, \"data\": %s}",
        message->action, message->data);
    return 0;
}

int deserialize_message(const char *buffer, Message *message) {
    sscanf(buffer,
        "{\"game_state\": %u, \"data\": %s}",
        message->action, message->data);

    return 0;
}
