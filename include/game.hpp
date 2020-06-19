#ifndef GAME_H
#define GAME_H

#include <memory>
#include <chrono>
#include "renderer.hpp"
#include "user_input.hpp"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

class Game
{
private:
    bool Running;
    UserInputState Input;
    SDLRenderer Render;

public:
    void Init();
    void Loop();
    void Close();
};

#endif // GAME_H
