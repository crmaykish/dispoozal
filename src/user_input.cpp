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
        else if (event.type == SDL_MOUSEMOTION)
        {
            // Continuously capture mouse position
            int x, y;
            SDL_GetMouseState(&x, &y);
            Cursor.x = x * (WORLDSIZE_W / RESOLUTION_W);
            Cursor.y = (RESOLUTION_H - y) * (WORLDSIZE_H / RESOLUTION_H);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                FireMain = true;
                break;
            case SDL_BUTTON_RIGHT:
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                FireMain = false;
                break;
            case SDL_BUTTON_RIGHT:
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                Up = true;
                break;
            case SDLK_s:
                Down = true;
                break;
            case SDLK_a:
                Left = true;
                break;
            case SDLK_d:
                Right = true;
                break;
            case SDLK_RETURN:
                FireMain = true;
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                Up = false;
                break;
            case SDLK_s:
                Down = false;
                break;
            case SDLK_a:
                Left = false;
                break;
            case SDLK_d:
                Right = false;
                break;
            case SDLK_RETURN:
                FireMain = false;
                break;
            }
        }
    }
}