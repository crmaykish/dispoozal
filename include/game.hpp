#ifndef GAME_H
#define GAME_H

#include <memory>
#include <chrono>
#include <vector>
#include "game_state.hpp"
#include "renderer.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "font.hpp"
#include "database.hpp"
#include "animation.hpp"
#include "sound.hpp"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

const int CASUAL_STARTING_RATE = 1000;
const float CASUAL_MOVE_SPEED = 3.0;
const int CASUAL_RATE_INCREASE = 20;
const int CASUAL_RATE_MIN = 400;

const int NORMAL_STARTING_RATE = 800;
const float NORMAL_MOVE_SPEED = 4.0;
const int NORMAL_RATE_INCREASE = 30;
const int NORMAL_RATE_MIN = 300;

const int INSANE_STARTING_RATE = 700;
const float INSANE_MOVE_SPEED = 5.0;
const int INSANE_RATE_INCREASE = 50;
const int INSANE_RATE_MIN = 250;

enum GameDifficulty
{
    DIFFICULTY_CASUAL = 0,
    DIFFICULTY_NORMAL = 1,
    DIFFICULTY_INSANE = 2,
};

struct DifficultySettings
{
    GameDifficulty Diff;
    int StartingRate;
    float MoveSpeed;
    int RateIncrease;
    int RateTimeoutMin;
};

class Game
{
private:
    bool Running;
    GameState State;
    SDLRenderer Renderer;
    Database DB;

    std::shared_ptr<MyFont> UIFont;

    std::shared_ptr<Texture> OverlayTexture;
    std::shared_ptr<Texture> EnemyTexture;
    std::shared_ptr<Texture> ScoreFrameTexture;
    std::shared_ptr<Texture> MainLogo;

    // UI

    Animation ButtonCasualAnimation;
    Animation ButtonNormalAnimation;
    Animation ButtonInsaneAnimation;
    Animation ButtonExitAnimation;
    Animation ButtonExitLargeAnimation;
    Animation ButtonRetryAnimation;

    Animation MainMenuAnimation;

    int MainMenuSelectedButtonIndex = 0;
    int GameoverMenuSelectedButtonIndex = 0;
    int PauseMenuSelectedButtonIndex = 0;

    // sounds
    std::shared_ptr<Sound> SuccessSound;
    std::shared_ptr<Sound> FailSound;
    std::shared_ptr<Sound> ClickSound;
    std::shared_ptr<Sound> SelectSound;
    std::shared_ptr<Sound> HighscoreSound;

    std::shared_ptr<MyMusic> SlowMusic;
    std::shared_ptr<MyMusic> MediumMusic;
    std::shared_ptr<MyMusic> FastMusic;

    std::shared_ptr<Player> PlayerOne;
    std::vector<std::shared_ptr<Enemy>> Enemies;

    DifficultySettings SelectedDifficulty;

    GameTimer FireTimer;
    int startingTimeout = 1000;
    EnemyDirection LastDirection = ENEMY_DIR_UNKNOWN;

    bool MenuButtonDownLast = false;
    bool ModeSwitch = false;

    bool HighscoreFlag = false;

    GameTimer CleanupTimer;

    void Update();
    void Render();
    void Cleanup();

    void Reset();

public:
    Game();

    void Init();
    void Loop();
    void Close();
};

#endif // GAME_H
