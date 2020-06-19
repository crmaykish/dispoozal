#include <algorithm>
#include <chrono>
#include "game.hpp"
#include "logger.hpp"
#include "player.hpp"
#include "spawner.hpp"

void Game::Init()
{
    Log("Initializing game", LOG_INFO);

    Renderer.Init();

    // Add player
    auto p1 = std::make_shared<Player>();
    p1->SetMainTexture(Renderer.LoadTexture("assets/player.png"));
    PlayerOne = p1;

    // Add enemy spawner
    auto spawner0 = std::make_shared<Spawner>(Point{100, 600}, Enemies, Projectiles);
    spawner0->SetMainTexture(Renderer.LoadTexture("assets/spawner.png"));
    spawner0->SetEnemyTexture(Renderer.LoadTexture("assets/enemy.png"));

    Spawners.push_back(spawner0);

    auto spawner1 = std::make_shared<Spawner>(Point{900, 600}, Enemies, Projectiles);
    spawner1->SetMainTexture(Renderer.LoadTexture("assets/spawner.png"));
    spawner1->SetEnemyTexture(Renderer.LoadTexture("assets/enemy.png"));

    Spawners.push_back(spawner1);
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
            Update();

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

            if (State.GetInput().Quit)
            {
                Running = false;
            }

            lag -= TIME_PER_TICK;
        }

        Render();
    }
}

void Game::Close()
{
    Log("Closing game", LOG_INFO);
}

void Game::Update()
{
    PlayerOne->Update(State);

    for (auto s : Spawners)
    {
        s->Update(State);
    }

    for (auto e : Enemies)
    {
        e->Update(State);
    }

    for (auto p : Projectiles)
    {
        p->Update(State);
    }
}

void Game::Render()
{
    Renderer.Clear();

    // Render objects
    PlayerOne->Render(Renderer);

    for (auto s : Spawners)
    {
        s->Render(Renderer);
    }

    for (auto e : Enemies)
    {
        e->Render(Renderer);
    }

    for (auto p : Projectiles)
    {
        p->Render(Renderer);
    }

    // Render cursor
    Point cursorPosition = State.GetInput().Cursor;
    Renderer.RenderRectangle({cursorPosition, float(TEXTURE_SCALE), float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);

    Renderer.Present();
}
