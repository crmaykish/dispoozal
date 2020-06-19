#include "game_state.hpp"

GameState::GameState()
{
}

UserInputState &GameState::GetInput()
{
    return Input;
}