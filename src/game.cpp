#include <algorithm>
#include <chrono>
#include "game.hpp"
#include "logger.hpp"
#include "player.hpp"
#include "rand_utils.hpp"

std::shared_ptr<Animation> CreateBarrelAnimation(std::shared_ptr<Texture> spriteSheet)
{
    return std::make_shared<Animation>(spriteSheet, 8, 28, 28, 80, false);
}

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
    UIFont = Renderer.LoadFont("assets/EXEPixelPerfect.ttf");
    OverlayTexture = Renderer.LoadTexture("assets/overlay.png");
    EnemyTexture = Renderer.LoadTexture("assets/barrel.png");
    ScoreFrameTexture = Renderer.LoadTexture("assets/score_frame.png");
    MainLogo = Renderer.LoadTexture("assets/logo.png");

    // load animations
    ButtonCasualAnimation = Animation(Renderer.LoadTexture("assets/button_casual.png"), 2, 47 * 4, 12 * 4, 0, false);
    ButtonNormalAnimation = Animation(Renderer.LoadTexture("assets/button_normal.png"), 2, 47 * 4, 12 * 4, 0, false);
    ButtonInsaneAnimation = Animation(Renderer.LoadTexture("assets/button_insane.png"), 2, 47 * 4, 12 * 4, 0, false);
    ButtonExitAnimation = Animation(Renderer.LoadTexture("assets/button_exit.png"), 2, 33 * 4, 12 * 4, 0, false);
    ButtonExitLargeAnimation = Animation(Renderer.LoadTexture("assets/button_exit_large.png"), 2, 47 * 4, 12 * 4, 0, false);
    ButtonRetryAnimation = Animation(Renderer.LoadTexture("assets/button_retry.png"), 2, 47 * 4, 12 * 4, 0, false);

    MainMenuAnimation = Animation(Renderer.LoadTexture("assets/player_down.png"), 3, 30, 23, 150, true);

    // load sounds
    SuccessSound = Renderer.LoadSound("assets/success.wav");
    FailSound = Renderer.LoadSound("assets/fail.wav");
    ClickSound = Renderer.LoadSound("assets/move.wav");
    SelectSound = Renderer.LoadSound("assets/select.wav");

    // load music
    SlowMusic = Renderer.LoadMusic("assets/theme_120.mp3");
    MediumMusic = Renderer.LoadMusic("assets/theme_130.mp3");
    FastMusic = Renderer.LoadMusic("assets/theme_140.mp3");

    // Add player
    PlayerOne = std::make_shared<Player>();
    PlayerOne->SetRightAnimation(std::make_shared<Animation>(Renderer.LoadTexture("assets/player_right.png"), 3, 30, 23, 150, true));
    PlayerOne->SetLeftAnimation(std::make_shared<Animation>(Renderer.LoadTexture("assets/player_left.png"), 3, 30, 23, 150, true));
    PlayerOne->SetUpAnimation(std::make_shared<Animation>(Renderer.LoadTexture("assets/player_up.png"), 3, 30, 23, 150, true));
    PlayerOne->SetDownAnimation(std::make_shared<Animation>(Renderer.LoadTexture("assets/player_down.png"), 3, 30, 23, 150, true));

    Reset();

    Renderer.PlayMusic(SlowMusic);
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
        MainMenuAnimation.Update();

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
                Renderer.PlaySound(ClickSound);

                MainMenuSelectedButtonIndex = MainMenuSelectedButtonIndex == 3 ? 0 : MainMenuSelectedButtonIndex + 1;
                MenuButtonDownLast = true;
            }
        }
        else if (State.Input.Up)
        {
            if (!MenuButtonDownLast)
            {
                Renderer.PlaySound(ClickSound);

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
            Renderer.PlaySound(SelectSound);

            // button selected, handle it
            if (MainMenuSelectedButtonIndex == 0)
            {
                // play casual
                SelectedDifficulty = {
                    DIFFICULTY_CASUAL,
                    CASUAL_STARTING_RATE,
                    CASUAL_MOVE_SPEED,
                    CASUAL_RATE_INCREASE,
                    CASUAL_RATE_MIN};

                Reset();
                State.Status = STATUS_RUNNING;

                Renderer.PlayMusic(SlowMusic);
            }
            else if (MainMenuSelectedButtonIndex == 1)
            {
                // play normal
                SelectedDifficulty = {
                    DIFFICULTY_NORMAL,
                    NORMAL_STARTING_RATE,
                    NORMAL_MOVE_SPEED,
                    NORMAL_RATE_INCREASE,
                    NORMAL_RATE_MIN};

                Reset();
                State.Status = STATUS_RUNNING;

                Renderer.PlayMusic(MediumMusic);
            }
            else if (MainMenuSelectedButtonIndex == 2)
            {
                // play insane
                SelectedDifficulty = {
                    DIFFICULTY_INSANE,
                    INSANE_STARTING_RATE,
                    INSANE_MOVE_SPEED,
                    INSANE_RATE_INCREASE,
                    INSANE_RATE_MIN};

                Reset();
                State.Status = STATUS_RUNNING;

                Renderer.PlayMusic(FastMusic);
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
                Renderer.PlaySound(ClickSound);

                GameoverMenuSelectedButtonIndex = GameoverMenuSelectedButtonIndex == 1 ? 0 : GameoverMenuSelectedButtonIndex + 1;
                MenuButtonDownLast = true;
            }
        }
        else if (State.Input.Up)
        {
            if (!MenuButtonDownLast)
            {
                Renderer.PlaySound(ClickSound);

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
            Renderer.PlaySound(SelectSound);

            if (GameoverMenuSelectedButtonIndex == 0)
            {
                // handle retry
                State.Status = STATUS_RUNNING;
            }
            else if (GameoverMenuSelectedButtonIndex == 1)
            {
                // handle exit
                State.Status = STATUS_MENU;
                ModeSwitch = true;
            }

            Reset();
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

            e = std::make_shared<Enemy>(p, SelectedDifficulty.MoveSpeed, CreateBarrelAnimation(EnemyTexture));
            e->Direction = dir;

            Enemies.push_back(e);

            // Update rate as level increases
            if (State.Score > 0 && State.Score % 5 == 0)
            {
                if (SelectedDifficulty.StartingRate <= SelectedDifficulty.RateTimeoutMin)
                {
                    // already at the fastest rate
                    SelectedDifficulty.StartingRate = SelectedDifficulty.RateTimeoutMin;
                }
                else
                {
                    SelectedDifficulty.StartingRate -= SelectedDifficulty.RateIncrease;
                }

                FireTimer.SetTimeout(SelectedDifficulty.StartingRate);

                Log(std::to_string(SelectedDifficulty.StartingRate));
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
                    Renderer.PlaySound(SuccessSound);

                    State.Score++;
                }
                else
                {
                    Renderer.PlayMusic(SlowMusic);
                    Renderer.PlaySound(FailSound);

                    State.Status = STATUS_GAMEOVER;

                    if (State.Score > State.BestScore)
                    {
                        // Update best score
                        State.BestScore = State.Score;

                        DB.AddScore(SelectedDifficulty.Diff, State.Score);
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
        Rectangle logoRect = {18 * TEXTURE_SCALE * 4, 57 * TEXTURE_SCALE * 4 + 120, 180 * 3 - 20, 100 * 2};
        Renderer.RenderWholeTexture(MainLogo, logoRect);

        MainMenuAnimation.Render(Renderer, 590, 585);

        // render buttons
        ButtonCasualAnimation.Render(Renderer, 18 * TEXTURE_SCALE * 4, 57 * TEXTURE_SCALE * 4);
        ButtonNormalAnimation.Render(Renderer, 18 * TEXTURE_SCALE * 4, 42 * TEXTURE_SCALE * 4);
        ButtonInsaneAnimation.Render(Renderer, 18 * TEXTURE_SCALE * 4, 27 * TEXTURE_SCALE * 4);
        ButtonExitAnimation.Render(Renderer, 35 * TEXTURE_SCALE * 4, 10 * TEXTURE_SCALE * 4);

        // render high scores
        Rectangle casualRect = {68 * TEXTURE_SCALE * 4, 57 * TEXTURE_SCALE * 4, 15 * TEXTURE_SCALE * 4, 12 * TEXTURE_SCALE * 4};
        Rectangle casualNumRect = casualRect;
        casualNumRect.pos.x += 24;
        casualNumRect.size.w -= 40;
        casualNumRect.size.h += 16;
        Renderer.RenderWholeTexture(ScoreFrameTexture, casualRect);
        Renderer.RenderFont(UIFont, std::to_string(State.BestScoreCasual), casualNumRect, FG_COLOR);

        Rectangle normalRect = {68 * TEXTURE_SCALE * 4, 42 * TEXTURE_SCALE * 4, 15 * TEXTURE_SCALE * 4, 12 * TEXTURE_SCALE * 4};
        Rectangle normalNumRect = normalRect;
        normalNumRect.pos.x += 24;
        normalNumRect.size.w -= 40;
        normalNumRect.size.h += 16;
        Renderer.RenderWholeTexture(ScoreFrameTexture, normalRect);
        Renderer.RenderFont(UIFont, std::to_string(State.BestScoreNormal), normalNumRect, FG_COLOR);

        Rectangle insaneRect = {68 * TEXTURE_SCALE * 4, 27 * TEXTURE_SCALE * 4, 15 * TEXTURE_SCALE * 4, 12 * TEXTURE_SCALE * 4};
        Rectangle insaneNumRect = insaneRect;
        insaneNumRect.pos.x += 24;
        insaneNumRect.size.w -= 40;
        insaneNumRect.size.h += 16;
        Renderer.RenderWholeTexture(ScoreFrameTexture, insaneRect);
        Renderer.RenderFont(UIFont, std::to_string(State.BestScoreInsane), insaneNumRect, FG_COLOR);
    }
    else if (State.Status == STATUS_GAMEOVER)
    {
        Renderer.RenderWholeTexture(OverlayTexture, {0, 0, 800, 800});

        // Render score
        float w = 160;
        float h = 40;
        float offset = 10;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "SCORE: " + std::to_string(State.Score), scoreRect, BG_COLOR);

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "BEST: " + std::to_string(State.BestScore), bestScoreRect, BG_COLOR);

        // render buttons
        ButtonRetryAnimation.Render(Renderer, 400 - 188, 400 + 40);
        ButtonExitLargeAnimation.Render(Renderer, 400 - 188, 400 - 44 - 40);
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
        float w = 160;
        float h = 40;
        float offset = 10;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "SCORE: " + std::to_string(State.Score), scoreRect, BG_COLOR);

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        Renderer.RenderFont(UIFont, "BEST: " + std::to_string(State.BestScore), bestScoreRect, BG_COLOR);
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
    State.BestScore = DB.GetHighScore(SelectedDifficulty.Diff);

    if (SelectedDifficulty.Diff == DIFFICULTY_CASUAL)
    {
        SelectedDifficulty.StartingRate = CASUAL_STARTING_RATE;
    }
    else if (SelectedDifficulty.Diff == DIFFICULTY_NORMAL)
    {
        SelectedDifficulty.StartingRate = NORMAL_STARTING_RATE;
    }
    if (SelectedDifficulty.Diff == DIFFICULTY_INSANE)
    {
        SelectedDifficulty.StartingRate = INSANE_STARTING_RATE;
    }

    State.BestScoreCasual = DB.GetHighScore(DIFFICULTY_CASUAL);
    State.BestScoreNormal = DB.GetHighScore(DIFFICULTY_NORMAL);
    State.BestScoreInsane = DB.GetHighScore(DIFFICULTY_INSANE);

    FireTimer.SetTimeout(SelectedDifficulty.StartingRate);
    FireTimer.Reset();

    Enemies.clear();
}
