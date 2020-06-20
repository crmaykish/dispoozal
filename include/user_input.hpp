#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include "game_geometry.hpp"
#include "vector.hpp"

struct UserInputState
{
    bool Quit = false;
    Point Cursor;

    bool Left = false;
    bool Right = false;
    bool Up = false;
    bool Down = false;
    bool Select = false;

    void PollForInput();
};

/** @brief Convert a combination of directions into a unit vector for movement at that angle */
Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R);

#endif // USER_INPUT_HPP
