#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "user_input.hpp"

class GameState
{
private:
    UserInputState Input;

public:
    GameState();
    UserInputState &GetInput();
};

#endif // GAME_STATE_H
