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

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

class Game
{
private:
    bool Running;
    GameState State;
    SDLRenderer Renderer;

    std::shared_ptr<MyFont> UIFont;

    std::shared_ptr<Texture> OverlayTexture;
    std::shared_ptr<Texture> EnemyTexture;

    std::shared_ptr<Player> PlayerOne;
    std::vector<std::shared_ptr<Enemy>> Enemies;

    GameTimer FireTimer;
    int startingTimeout = 1000;

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
