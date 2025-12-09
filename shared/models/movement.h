#ifndef SHARED_MODELS_MOVEMENT_H
#define SHARED_MODELS_MOVEMENT_H

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
} MoveDirection;

typedef struct {
    PlayerId player_id;
    MoveDirection direction;
} MoveData;

#endif
