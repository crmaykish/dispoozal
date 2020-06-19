#include <algorithm>
#include <chrono>
#include "game.hpp"
#include "logger.hpp"
#include "player.hpp"
#include "spawner.hpp"

Game::Game() : CleanupTimer(5000)
{
    CleanupTimer.Reset();
}

void Game::Init()
{
    Log("Initializing game", LOG_INFO);

    Renderer.Init();

    UIFont = Renderer.LoadFont("assets/BitPap.ttf");

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

void Game::Update()
{
    if (State.Status != STATUS_RUNNING)
    {
        return;
    }

    // Update game objects

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

        Point n = p->GetPosition();
        if (n.x < 0 || n.x > WORLDSIZE_W || n.y < 0 || n.y > WORLDSIZE_H)
        {
            p->Deactivate();
        }
    }

    // Check for collisions

    for (auto p : Projectiles)
    {
        if (p->IsActive())
        {
            if (p->Friendly)
            {
                HandleEnemyCollisions(p);
                HandleSpawnerCollisions(p);
            }
            else
            {
                HandlePlayerCollisions(p);
            }
        }
    }

    // TODO: check for player/enemy collisions

    // Is the player still alive?
    if (!PlayerOne->IsAlive())
    {
        State.Status = STATUS_GAMEOVER;
    }

    // move any created projectiles to the main list
    Projectiles.insert(Projectiles.end(),
                       std::make_move_iterator(PlayerProjectiles.begin()),
                       std::make_move_iterator(PlayerProjectiles.end()));
    PlayerProjectiles.clear();
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

    // Render game over screen
    if (State.Status == STATUS_GAMEOVER)
    {
        Rectangle textRectangle = {(WORLDSIZE_W - 600) / 2, (WORLDSIZE_H - 100) / 2, 600, 100};

        Renderer.RenderFont(UIFont, "GAME OVER", textRectangle);
    }

    // Render cursor
    Point cursorPosition = State.GetInput().Cursor;
    Renderer.RenderRectangle({cursorPosition, float(TEXTURE_SCALE), float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);

    Renderer.Present();
}

void Game::Cleanup()
{
    // Cleanup deactivated objects

    for (int i = 0; i < Spawners.size(); i++)
    {
        Spawners.erase(
            std::remove_if(Spawners.begin(),
                           Spawners.end(),
                           [](std::shared_ptr<Spawner> o) { return !o->IsActive(); }),
            Spawners.end());
    }

    for (int i = 0; i < Enemies.size(); i++)
    {
        Enemies.erase(
            std::remove_if(Enemies.begin(),
                           Enemies.end(),
                           [](std::shared_ptr<Enemy> o) { return !o->IsActive(); }),
            Enemies.end());
    }

    for (int i = 0; i < Projectiles.size(); i++)
    {
        Projectiles.erase(
            std::remove_if(Projectiles.begin(),
                           Projectiles.end(),
                           [](std::shared_ptr<Projectile> o) { return !o->IsActive(); }),
            Projectiles.end());
    }
}

void Game::Reset()
{
    Spawners.clear();
    Enemies.clear();
    Projectiles.clear();

    // Add enemy spawner
    auto spawner0 = std::make_shared<Spawner>(Point{100, 600}, Enemies, Projectiles);
    spawner0->SetMainTexture(Renderer.LoadTexture("assets/spawner.png"));
    spawner0->SetEnemyTexture(Renderer.LoadTexture("assets/enemy.png"));
    Spawners.push_back(spawner0);

    auto spawner1 = std::make_shared<Spawner>(Point{800, 200}, Enemies, Projectiles);
    spawner1->SetMainTexture(Renderer.LoadTexture("assets/spawner.png"));
    spawner1->SetEnemyTexture(Renderer.LoadTexture("assets/enemy.png"));
    Spawners.push_back(spawner1);
}

void Game::HandlePlayerCollisions(std::shared_ptr<Projectile> projectile)
{
    if (RectangleCollision(PlayerOne->GetHitBox(), projectile->GetHitBox()))
    {
        projectile->Deactivate();

        // Determine angle of hit
        float projectileAngle = projectile->GetVelocity().GetAngle();

        if (projectileAngle < 0.0)
        {
            projectileAngle += Pi;
        }
        else
        {
            projectileAngle -= Pi;
        }

        // if player angle is between -45 and 45 degrees of the projectile angle, reflect, else damage

        float minAngle = projectileAngle - Pi / 4;
        float maxAngle = projectileAngle + Pi / 4;

        if (PlayerOne->GetRotation() > minAngle &&
            PlayerOne->GetRotation() < maxAngle)
        {
            if (PlayerOne->GetFlip())
            {
                // refract projectiles
                for (int i = 0; i < 6; i++)
                {
                    // TODO: fix this refraction math
                    Vector2D refractionVector = Vector2D(10.0, projectile->GetVelocity().GetAngle() + ((i * projectile->GetVelocity().GetAngle()) / 6));
                    auto refractedProjectile = std::make_shared<Projectile>(PlayerOne->GetHitBox().pos, refractionVector);
                    refractedProjectile->Friendly = true;

                    PlayerProjectiles.push_back(refractedProjectile);
                }
            }
            else
            {
                // reflect projectile
                Vector2D reflectionVector = Vector2D(10, PlayerOne->GetRotation());
                auto reflectedProjectile = std::make_shared<Projectile>(PlayerOne->GetPosition(), reflectionVector);
                reflectedProjectile->Friendly = true;

                PlayerProjectiles.push_back(reflectedProjectile);
            }
        }
        else
        {
            PlayerOne->Damage(10);
        }
    }
}

void Game::HandleEnemyCollisions(std::shared_ptr<Projectile> projectile)
{
    for (auto e : Enemies)
    {
        if (RectangleCollision(projectile->GetHitBox(), e->GetHitBox()))
        {
            projectile->Deactivate();

            e->Damage(10);
        }
    }
}

void Game::HandleSpawnerCollisions(std::shared_ptr<Projectile> projectile)
{
    for (auto s : Spawners)
    {
        if (RectangleCollision(projectile->GetHitBox(), s->GetHitBox()))
        {
            projectile->Deactivate();

            s->Damage(10);
        }
    }
}