#include "game_timer.hpp"

GameTimer::GameTimer(int timeoutMS)
{
    Timeout = std::chrono::duration<double>(timeoutMS / 1000.0);

    Reset();
}

void GameTimer::Reset()
{
    StartTime = std::chrono::steady_clock::now();
}

bool GameTimer::IsExpired() {
    auto elapsedTime = std::chrono::steady_clock::now() - StartTime;
    return elapsedTime >= Timeout;
}