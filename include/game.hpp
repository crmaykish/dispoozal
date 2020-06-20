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

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

enum GameDifficulty
{
    DIFFICULTY_CASUAL = 0,
    DIFFICULTY_NORMAL = 1,
    DIFFICULTY_INSANE = 2,
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

    // UI

    Animation ButtonCasualAnimation;
    Animation ButtonNormalAnimation;
    Animation ButtonInsaneAnimation;
    Animation ButtonExitAnimation;
    Animation ButtonExitLargeAnimation;
    Animation ButtonRetryAnimation;

    int MainMenuSelectedButtonIndex = 0;
    int GameoverMenuSelectedButtonIndex = 0;
    int PauseMenuSelectedButtonIndex = 0;

    std::shared_ptr<Player> PlayerOne;
    std::vector<std::shared_ptr<Enemy>> Enemies;

    GameDifficulty SelectedDifficulty = DIFFICULTY_NORMAL;

    GameTimer FireTimer;
    int startingTimeout = 1000;
    EnemyDirection LastDirection = ENEMY_DIR_UNKNOWN;

    bool MenuButtonDownLast = false;
    bool ModeSwitch = false;

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
