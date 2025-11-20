#ifndef SHARED_MODELS_MESSAGE_H
#define SHARED_MODELS_MESSAGE_H

typedef enum MessageAction {
    SERVER_FULL,
    SERVER_ACCEPTED,
    GAME_STATE_CHANGED,
    PLAYER_ASSIGNED_ID,
    PLAYER_JOINED,
    PLAYER_LEFT,
    PLAYER_MOVED,
} MessageAction;

typedef struct Message {
    MessageAction action;
    char data[256];
} Message;

#endif
