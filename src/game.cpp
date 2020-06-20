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

    // Seed the random number generator
    srand(time(NULL));

    DB.Open();

    Renderer.Init();

    // Load resources
    UIFont = Renderer.LoadFont("assets/BitPap.ttf");
    OverlayTexture = Renderer.LoadTexture("assets/overlay.png");
    EnemyTexture = Renderer.LoadTexture("assets/enemy.png");
    auto playerTexture = Renderer.LoadTexture("assets/player.png");

    // load animations
    ButtonCasualAnimation = Animation(Renderer.LoadTexture("assets/button_casual.png"), 2, 47, 12, 0, false);
    ButtonNormalAnimation = Animation(Renderer.LoadTexture("assets/button_normal.png"), 2, 47, 12, 0, false);
    ButtonInsaneAnimation = Animation(Renderer.LoadTexture("assets/button_insane.png"), 2, 47, 12, 0, false);
    ButtonExitAnimation = Animation(Renderer.LoadTexture("assets/button_exit.png"), 2, 33, 12, 0, false);
    ButtonExitLargeAnimation = Animation(Renderer.LoadTexture("assets/button_exit_large.png"), 2, 47, 12, 0, false);
    ButtonRetryAnimation = Animation(Renderer.LoadTexture("assets/button_retry.png"), 2, 47, 12, 0, false);

    // Add player
    PlayerOne = std::make_shared<Player>();
    PlayerOne->SetMainTexture(playerTexture);

    Reset();
}

void Game::Loop()
{
    Log("Starting game loop", LOG_INFO);

    Running = true;
    State.Status = STATUS_MENU;

    auto previousTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> lag;

    while (Running)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsedTime;

        State.Input.PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            Update();

            if (CleanupTimer.IsExpired())
            {
                Cleanup();
                CleanupTimer.Reset();
            }

            if (State.Input.Quit)
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
    if (State.Status == STATUS_MENU)
    {
        if (ModeSwitch && State.Input.Select)
        {
            return;
        }

        ModeSwitch = false;

        // Change the selected button index
        if (State.Input.Down)
        {
            if (!MenuButtonDownLast)
            {
                MainMenuSelectedButtonIndex = MainMenuSelectedButtonIndex == 3 ? 0 : MainMenuSelectedButtonIndex + 1;
                MenuButtonDownLast = true;
            }
        }
        else if (State.Input.Up)
        {
            if (!MenuButtonDownLast)
            {
                MainMenuSelectedButtonIndex = MainMenuSelectedButtonIndex == 0 ? 3 : MainMenuSelectedButtonIndex - 1;
                MenuButtonDownLast = true;
            }
        }
        else
        {
            MenuButtonDownLast = false;
        }

        //reset all buttons
        // TODO: this doesn't need to run every frame
        ButtonCasualAnimation.SetFrame(0);
        ButtonNormalAnimation.SetFrame(0);
        ButtonInsaneAnimation.SetFrame(0);
        ButtonExitAnimation.SetFrame(0);

        if (MainMenuSelectedButtonIndex == 0)
        {
            ButtonCasualAnimation.SetFrame(1);
        }
        else if (MainMenuSelectedButtonIndex == 1)
        {
            ButtonNormalAnimation.SetFrame(1);
        }
        else if (MainMenuSelectedButtonIndex == 2)
        {
            ButtonInsaneAnimation.SetFrame(1);
        }
        else if (MainMenuSelectedButtonIndex == 3)
        {
            ButtonExitAnimation.SetFrame(1);
        }

        if (State.Input.Select)
        {
            // button selected, handle it
            if (MainMenuSelectedButtonIndex == 0)
            {
                // play casual
                SelectedDifficulty = DIFFICULTY_CASUAL;
                Reset();
                State.Status = STATUS_RUNNING;
            }
            else if (MainMenuSelectedButtonIndex == 1)
            {
                // play normal
                SelectedDifficulty = DIFFICULTY_NORMAL;
                Reset();
                State.Status = STATUS_RUNNING;
            }
            else if (MainMenuSelectedButtonIndex == 2)
            {
                // play insane
                SelectedDifficulty = DIFFICULTY_INSANE;
                Reset();
                State.Status = STATUS_RUNNING;
            }
            else if (MainMenuSelectedButtonIndex == 3)
            {
                //exit
                Running = false;
            }
        }
    }
    else if (State.Status == STATUS_GAMEOVER)
    {
        // Change the selected button index
        if (State.Input.Down)
        {
            if (!MenuButtonDownLast)
            {
                GameoverMenuSelectedButtonIndex = GameoverMenuSelectedButtonIndex == 1 ? 0 : GameoverMenuSelectedButtonIndex + 1;
                MenuButtonDownLast = true;
            }
        }
        else if (State.Input.Up)
        {
            if (!MenuButtonDownLast)
            {
                GameoverMenuSelectedButtonIndex = GameoverMenuSelectedButtonIndex == 0 ? 1 : GameoverMenuSelectedButtonIndex - 1;
                MenuButtonDownLast = true;
            }
        }
        else
        {
            MenuButtonDownLast = false;
        }

        //reset all buttons
        // TODO: this doesn't need to run every frame
        ButtonRetryAnimation.SetFrame(0);
        ButtonExitLargeAnimation.SetFrame(0);

        if (GameoverMenuSelectedButtonIndex == 0)
        {
            ButtonRetryAnimation.SetFrame(1);
        }
        else if (GameoverMenuSelectedButtonIndex == 1)
        {
            ButtonExitLargeAnimation.SetFrame(1);
        }

        if (State.Input.Select)
        {
            // button selected, handle it
            if (GameoverMenuSelectedButtonIndex == 0)
            {
                // handle retry
                Reset();
                State.Status = STATUS_RUNNING;
            }
            else if (GameoverMenuSelectedButtonIndex == 1)
            {
                // handle exit
                State.Status = STATUS_MENU;
                ModeSwitch = true;
            }
        }
    }
    else if (State.Status == STATUS_RUNNING)
    {
        // Update game objects

        PlayerOne->Update(State);

        if (FireTimer.IsExpired())
        {
            // Fire from a random direction
            Point p;
            EnemyDirection dir = LastDirection;
            std::shared_ptr<Enemy> e = nullptr;

            // Never repeat directions
            while (dir == LastDirection)
            {
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
                else if (rand == 3)
                {
                    p = {400, -20};
                    dir = ENEMY_UP;
                }
            }

            LastDirection = dir;

            e = std::make_shared<Enemy>(p);
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

                    if (State.Score > State.BestScore)
                    {
                        // Update best score
                        State.BestScore = State.Score;

                        DB.AddScore(SelectedDifficulty, State.Score);
                    }
                }

                e->Deactivate();
            }
        }
    }
}

void Game::Render()
{
    Renderer.Clear();

    if (State.Status == STATUS_MENU)
    {
        // render title

        // render buttons
        ButtonCasualAnimation.Render(Renderer, 18 * TEXTURE_SCALE, 57 * TEXTURE_SCALE);
        ButtonNormalAnimation.Render(Renderer, 18 * TEXTURE_SCALE, 42 * TEXTURE_SCALE);
        ButtonInsaneAnimation.Render(Renderer, 18 * TEXTURE_SCALE, 27 * TEXTURE_SCALE);
        ButtonExitAnimation.Render(Renderer, 35 * TEXTURE_SCALE, 10 * TEXTURE_SCALE);
    }
    else if (State.Status == STATUS_GAMEOVER)
    {
        Renderer.RenderWholeTexture(OverlayTexture, {0, 0, 800, 800});

        // Render score
        float w = 120;
        float h = 60;
        float offset = 20;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "SCORE: " + std::to_string(State.Score), scoreRect);

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "BEST: " + std::to_string(State.BestScore), bestScoreRect);

        // render buttons

        ButtonRetryAnimation.Render(Renderer, 18 * TEXTURE_SCALE, 50 * TEXTURE_SCALE);
        ButtonExitLargeAnimation.Render(Renderer, 18 * TEXTURE_SCALE, 25 * TEXTURE_SCALE);
    }
    else if (State.Status == STATUS_RUNNING)
    {
        PlayerOne->Render(Renderer);

        for (auto e : Enemies)
        {
            e->Render(Renderer);
        }

        Renderer.RenderWholeTexture(OverlayTexture, {0, 0, 800, 800});

        // Render score
        float w = 120;
        float h = 60;
        float offset = 20;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "SCORE: " + std::to_string(State.Score), scoreRect);

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "BEST: " + std::to_string(State.BestScore), bestScoreRect);
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
    State.Score = 0;
    State.BestScore = DB.GetHighScore(SelectedDifficulty);

    FireTimer.SetTimeout(1000);
    FireTimer.Reset();

    Enemies.clear();
}
