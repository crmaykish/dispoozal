#ifndef GAME_H
#define GAME_H

#include <memory>
#include <chrono>
#include <vector>
#include "game_state.hpp"
#include "renderer.hpp"
#include "game_object.hpp"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

class Game
{
private:
    bool Running;
    GameState State;
    SDLRenderer Render;
    std::vector<std::shared_ptr<GameObject>> Objects;

public:
    void Init();
    void Loop();
    void Close();
};

#endif // GAME_H
