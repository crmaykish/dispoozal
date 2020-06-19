#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <memory>
#include "renderer.hpp"
#include "texture.hpp"
#include "game_timer.hpp"

class Animation
{
private:
    std::shared_ptr<Texture> SpriteSheet = nullptr;
    int FrameCount = 0;
    int FrameWidth = 0;
    int FrameHeight = 0;
    int CurrentFrame = 0;
    int Direction = 1;
    int OffSetX = 0;
    int OffSetY = 0;
    GameTimer Timer;
    bool PingPong = false;
    Point Center;
    double Rotation = 0.0;

public:
    Animation(std::shared_ptr<Texture> spriteSheet, int frameCount, int frameWidth, int frameHeight, int frameTime, bool pingPong = false);
    void Update();
    void Render(std::shared_ptr<SDLRenderer> renderer, int x, int y);

    void SetOffset(int x, int y);
    void SetCenter(Point center);
    void SetRotation(double angle);
};

#endif // ANIMATION_HPP
