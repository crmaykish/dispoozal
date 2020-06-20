#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <memory>
#include "user_input.hpp"

enum GameStatus
{
    STATUS_UNKNOWN,
    STATUS_MENU,
    STATUS_RUNNING,
    STATUS_GAMEOVER
};

struct GameState
{
    UserInputState Input;
    GameStatus Status = STATUS_UNKNOWN;
    int Score = 0;
    int BestScore = 0;
};

#endif // GAME_STATE_H
