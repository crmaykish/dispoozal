#include <chrono>
#include "game.hpp"
#include "logger.hpp"

void Game::Init()
{
    Log("Initializing game", LOG_INFO);

    Render.Init();
}

void Game::Loop()
{
    Log("Starting game loop", LOG_INFO);

    Running = true;

    auto previousTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> lag;

    while (Running)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsedTime;

        Input.PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            // TODO: Update game state based on user input
            // TODO: Update game objects

            // Cleanup inactive objects
            // for (int i = 0; i < Objects.size(); i++)
            // {
            //     // TODO: Don't do this every tick, once every few seconds is probably enough
            //     Objects.erase(
            //         std::remove_if(Objects.begin(),
            //                        Objects.end(),
            //                        [](std::shared_ptr<GameObject> o) { return !o->IsActive(); }),
            //         Objects.end());
            // }

            if (Input.Quit)
            {
                Running = false;
            }

            lag -= TIME_PER_TICK;
        }

        // RENDER

        Render.Clear();

        // TODO: render objects

        // Draw cursor
        Render.RenderRectangle({Input.Cursor, float(TEXTURE_SCALE), float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);

        Render.Present();
    }
}

void Game::Close()
{
    Log("Closing game", LOG_INFO);
}