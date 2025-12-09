#ifndef SHARED_MODELS_GAME_STATE_H
#define SHARED_MODELS_GAME_STATE_H

typedef enum {
    GAME_STATE_WAITING,
    GAME_STATE_RUNNING,
    GAME_STATE_OVER
} GameState;

typedef struct GameCtx
{
    int player_data_ptr;
    int time_seconds;
    GameState state;
} GameCtx;

#endif
