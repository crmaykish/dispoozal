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

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

class Game
{
private:
    bool Running;
    GameState State;
    SDLRenderer Renderer;
    std::shared_ptr<Player> PlayerOne;
    std::vector<std::shared_ptr<Spawner>> Spawners;
    std::vector<std::shared_ptr<Enemy>> Enemies;
    std::vector<std::shared_ptr<Projectile>> Projectiles;

    void Update();
    void Render();

public:
    void Init();
    void Loop();
    void Close();
};

#endif // GAME_H
