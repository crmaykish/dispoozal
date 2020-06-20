#include <SDL2/SDL.h>
#include "user_input.hpp"
#include "settings.hpp"

void UserInputState::PollForInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            Quit = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
            case SDLK_UP:
                Up = true;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                Down = true;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                Left = true;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                Right = true;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                Select = true;
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
            case SDLK_UP:
                Up = false;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                Down = false;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                Left = false;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                Right = false;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                Select = false;
                break;
            }
        }
    }
}

Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R)
{
    float magnitude = 1.0;
    float angle = 0.0;

    if ((R && L) || U && D)
    {
        magnitude = 0.0;
    }
    else if (R)
    {
        angle = 0.0 * TO_RADS;
    }
    else if (U)
    {
        angle = 90.0 * TO_RADS;
    }
    else if (L)
    {
        angle = 180.0 * TO_RADS;
    }
    else if (D)
    {
        angle = -90.0 * TO_RADS;
    }
    else
    {
        magnitude = 0.0;
        angle = 0.0;
    }

    return Vector2D(magnitude, angle);
}