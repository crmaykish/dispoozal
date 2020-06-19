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
    Point PlayerPosition; // This is really more like the enemy target destination
public:
    GameStatus Status = STATUS_UNKNOWN;

    GameState();
    UserInputState &GetInput();

    void SetPlayerPosition(Point position);
    Point GetPlayerPosition();
};

#endif // GAME_STATE_H
