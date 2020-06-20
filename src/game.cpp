#include <algorithm>
#include <chrono>
#include "game.hpp"
#include "logger.hpp"
#include "player.hpp"
#include "rand_utils.hpp"

Game::Game() : CleanupTimer(5000), FireTimer(1000)
{
    CleanupTimer.Reset();
    FireTimer.Reset();
}

void Game::Init()
{
    Log("Initializing game", LOG_INFO);

    Renderer.Init();

    UIFont = Renderer.LoadFont("assets/BitPap.ttf");

    OverlayTexture = Renderer.LoadTexture("assets/overlay.png");
    EnemyTexture = Renderer.LoadTexture("assets/enemy.png");

    // Add player
    PlayerOne = std::make_shared<Player>();
    PlayerOne->SetMainTexture(Renderer.LoadTexture("assets/player.png"));

    Reset();
}

void Game::Loop()
{
    Log("Starting game loop", LOG_INFO);

    Running = true;
    State.Status = STATUS_RUNNING;

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

            if (CleanupTimer.IsExpired())
            {
                Cleanup();
                CleanupTimer.Reset();
            }

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

bool CloseEnough(float a, float b)
{
    return (a > (b - 0.00001) && a < (b + 0.00001));
}

void Game::Update()
{
    if (State.Status != STATUS_RUNNING)
    {
        if (State.GetInput().FireMain)
        {
            Reset();
            State.Status = STATUS_RUNNING;
        }

        return;
    }

    // Update game objects

    PlayerOne->Update(State);

    if (FireTimer.IsExpired())
    {
        // Fire from a random direction
        Point p = {400, -20};
        EnemyDirection dir = ENEMY_UP;

        int rand = RandomInt(4);

        if (rand == 0)
        {
            p = {-20, 400};
            dir = ENEMY_RIGHT;
        }
        else if (rand == 1)
        {
            p = {400, 820};
            dir = ENEMY_DOWN;
        }
        else if (rand == 2)
        {
            p = {820, 400};
            dir = ENEMY_LEFT;
        }

        auto e = std::make_shared<Enemy>(p);
        e->SetMainTexture(EnemyTexture);
        e->Direction = dir;

        Enemies.push_back(e);

        // Update rate as level increases
        if (State.Score % 10 && State.Score < 80)
        {
            FireTimer.SetTimeout(startingTimeout - State.Score * 10);
        }

        FireTimer.Reset();
    }

    for (auto e : Enemies)
    {
        e->Update(State);
    }

    // Check for collisions
    for (auto e : Enemies)
    {
        if (e->IsActive() && RectangleCollision(PlayerOne->GetHitBox(), e->GetHitBox()))
        {
            EnemyDirection d = e->Direction;
            float rot = PlayerOne->GetRotation();

            // this is gross
            if ((d == ENEMY_LEFT && CloseEnough(rot, 0.0)) ||
                (d == ENEMY_UP && CloseEnough(rot, -Pi / 2)) ||
                (d == ENEMY_RIGHT && CloseEnough(rot, Pi)) ||
                (d == ENEMY_DOWN && CloseEnough(rot, Pi / 2)))
            {
                State.Score++;
            }
            else
            {
                State.Status = STATUS_GAMEOVER;
            }

            e->Deactivate();
        }
    }
}

void Game::Render()
{
    Renderer.Clear();

    // Render objects
    PlayerOne->Render(Renderer);

    for (auto e : Enemies)
    {
        e->Render(Renderer);
    }

    Renderer.RenderWholeTexture(OverlayTexture, {0, 0, 800, 800});

    // Render score
    Rectangle scoreRectangle = {(WORLDSIZE_W - 80) - 20, (WORLDSIZE_H - 60) - 20, 80, 60};
    Renderer.RenderFont(UIFont, std::to_string(State.Score), scoreRectangle);

    // Rectangle levelRect = {20, (WORLDSIZE_H - 60) - 20, 80, 60};
    // Renderer.RenderFont(UIFont, std::to_string(State.Level), levelRect);

    // Render game over screen
    if (State.Status == STATUS_GAMEOVER)
    {
        Rectangle textRectangle = {100, 350, 600, 100};
        Rectangle outline = {80, 350, 620, 100};

        Renderer.RenderRectangle(outline, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.a);
        Renderer.RenderFont(UIFont, "TRY AGAIN?", textRectangle);
    }

    Renderer.Present();
}

void Game::Cleanup()
{
    // Cleanup deactivated objects
    for (int i = 0; i < Enemies.size(); i++)
    {
        Enemies.erase(
            std::remove_if(Enemies.begin(),
                           Enemies.end(),
                           [](std::shared_ptr<Enemy> o) { return !o->IsActive(); }),
            Enemies.end());
    }
}

void Game::Reset()
{
    State.Level = 1;
    State.Score = 0;

    Enemies.clear();
}
