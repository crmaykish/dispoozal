#include "game_state.hpp"

GameState::GameState()
{
}

UserInputState &GameState::GetInput()
{
    return Input;
}

void GameState::SetPlayerPosition(Point position)
{
    PlayerPosition = position;
}

Point GameState::GetPlayerPosition()
{
    return PlayerPosition;
}
