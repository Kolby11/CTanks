#ifndef SHARED_MODELS_MOVEMENT_H
#define SHARED_MODELS_MOVEMENT_H

typedef struct Vector2 {
    int x;
    int y;
} Vector2;

typedef struct {
    PlayerId player_id;
    Vector2 direction;
} MoveData;

#endif
