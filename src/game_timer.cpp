#include "game_timer.hpp"

GameTimer::GameTimer(int timeoutMS)
{
    SetTimeout(timeoutMS);
    Reset();
}

void GameTimer::Reset()
{
    StartTime = std::chrono::steady_clock::now();
}

bool GameTimer::IsExpired()
{
    auto elapsedTime = std::chrono::steady_clock::now() - StartTime;
    return elapsedTime >= Timeout;
}

void GameTimer::SetTimeout(int timeoutMS)
{
    Timeout = std::chrono::duration<double>(timeoutMS / 1000.0);
}
