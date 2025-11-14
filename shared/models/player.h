#ifndef SHARED_DATA_PLAYER_H
#define SHARED_DATA_PLAYER_H

typedef int PlayerId;

typedef struct PlayerPosition {
    int x;
    int y;
} PlayerPosition;

typedef struct Player {
    PlayerId id;
    int sock;
    PlayerPosition position;
} Player;

#endif

