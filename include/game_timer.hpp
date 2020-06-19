#ifndef GAME_TIMER_HPP
#define GAME_TIMER_HPP

#include <chrono>

class GameTimer
{
private:
    // TODO: double is probably overkill for most usecases
    std::chrono::duration<double> Timeout;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> StartTime;

public:
    GameTimer(int timeoutMS);
    void Reset();
    bool IsExpired();
};

#endif // GAME_TIMER_HPP
