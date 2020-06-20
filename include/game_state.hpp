#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <memory>
#include "user_input.hpp"

enum GameStatus
{
    STATUS_UNKNOWN,
    STATUS_RUNNING,
    STATUS_GAMEOVER
};

class GameState
{
private:
    UserInputState Input;
public:
    GameStatus Status = STATUS_UNKNOWN;

    int Score = 0;
    int Level = 1;

    GameState();
    UserInputState &GetInput();
};

#endif // GAME_STATE_H
