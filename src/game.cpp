#include <algorithm>
#include <chrono>
#include "game.hpp"
#include "logger.hpp"
#include "player.hpp"

void Game::Init()
{
    Log("Initializing game", LOG_INFO);

    Render.Init();

    // Add player
    auto player = std::make_shared<Player>();
    player->SetMainTexture(Render.LoadTexture("assets/player.png"));
    Objects.push_back(player);
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

        State.GetInput().PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            // TODO: Update game state based on user input
            // TODO: Update game objects

            for (auto o : Objects)
            {
                o->Update(State);
            }

            // Cleanup inactive objects
            for (int i = 0; i < Objects.size(); i++)
            {
                // TODO: Don't do this every tick, once every few seconds is probably enough
                Objects.erase(
                    std::remove_if(Objects.begin(),
                                   Objects.end(),
                                   [](std::shared_ptr<GameObject> o) { return !o->IsActive(); }),
                    Objects.end());
            }

            if (State.GetInput().Quit)
            {
                Running = false;
            }

            lag -= TIME_PER_TICK;
        }

        // RENDER

        Render.Clear();

        for (auto &o : Objects)
        {
            o->Render(Render);
        }

        // Draw cursor
        Point cursorPosition = State.GetInput().Cursor;
        Render.RenderRectangle({cursorPosition, float(TEXTURE_SCALE), float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);

        Render.Present();
    }
}

void Game::Close()
{
    Log("Closing game", LOG_INFO);
}