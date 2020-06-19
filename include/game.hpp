#ifndef GAME_H
#define GAME_H

#include <memory>
#include <chrono>
#include <vector>
#include "game_state.hpp"
#include "renderer.hpp"
#include "player.hpp"
#include "spawner.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
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

    std::shared_ptr<Player> PlayerOne;
    std::vector<std::shared_ptr<Spawner>> Spawners;
    std::vector<std::shared_ptr<Enemy>> Enemies;
    std::vector<std::shared_ptr<Projectile>> Projectiles;
    std::vector<std::shared_ptr<Projectile>> PlayerProjectiles;

    GameTimer CleanupTimer;

    void Update();
    void Render();
    void Cleanup();

    void Reset();

    void HandlePlayerCollisions(std::shared_ptr<Projectile> projectile);
    void HandleEnemyCollisions(std::shared_ptr<Projectile> projectile);
    void HandleSpawnerCollisions(std::shared_ptr<Projectile> projectile);

public:
    Game();

    void Init();
    void Loop();
    void Close();
};

#endif // GAME_H
