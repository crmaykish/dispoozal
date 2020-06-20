#include "animation.hpp"
#include "logger.hpp"
#include "settings.hpp"

Animation::Animation() : Timer(100)
{
}

Animation::Animation(std::shared_ptr<Texture> spriteSheet, int frameCount, int frameWidth, int frameHeight, int frameTime, bool pingPong)
    : SpriteSheet(spriteSheet), FrameCount(frameCount), FrameWidth(frameWidth), FrameHeight(frameHeight), Timer(frameTime), PingPong(pingPong)
{
    Center = {0, 0};
    Rotation = 0.0;
}

void Animation::Update()
{
    if (FrameCount == 1)
    {
        CurrentFrame = 0;
        return;
    }

    if (Timer.IsExpired())
    {
        if (PingPong)
        {
            if (CurrentFrame == 0)
            {
                Direction = 1;
            }
            else if (CurrentFrame == FrameCount - 1)
            {
                Direction = -1;
            }
        }
        else
        {
            if (CurrentFrame == FrameCount - 1)
            {
                CurrentFrame = 0;
            }
        }

        CurrentFrame = CurrentFrame + Direction;
        Timer.Reset();
    }
}

void Animation::Render(SDLRenderer &renderer, int x, int y)
{
    Rectangle srcRect = {
        0,
        float(CurrentFrame * FrameHeight),
        float(FrameWidth),
        float(FrameHeight)};

    Rectangle destRect = {float(x + OffSetX * TEXTURE_SCALE),
                          float(y + OffSetY * TEXTURE_SCALE),
                          float(FrameWidth * TEXTURE_SCALE),
                          float(FrameHeight * TEXTURE_SCALE)};

    renderer.RenderTextureRotate(SpriteSheet, srcRect, destRect, Rotation * TO_DEGS, Center);
}

void Animation::SetFrame(int frame)
{
    CurrentFrame = frame;
}

void Animation::SetOffset(int x, int y)
{
    OffSetX = x;
    OffSetY = y;
}

void Animation::SetCenter(Point center)
{
    Center = center;
}

void Animation::SetRotation(double angle)
{
    Rotation = angle;
}