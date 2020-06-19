#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include "game_geometry.hpp"

struct UserInputState
{
    bool Quit = false;
    Point Cursor;

    bool Left = false;
    bool Right = false;
    bool Up = false;
    bool Down = false;
    bool FireMain = false;
    bool FireSecondary = false;

    void PollForInput();
};

#endif // USER_INPUT_HPP
