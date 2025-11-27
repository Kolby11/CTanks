#ifndef SHARED_SOCKET_H
#define SHARED_SOCKET_H

#include <netinet/in.h>

#include "shared/models/client.h"
#include "shared/models/message.h"

int receive_message(Client *client);
int send_message(Client *client, MessageType message_type, void *data, size_t data_size);

#endif