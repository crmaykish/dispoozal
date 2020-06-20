#include "player.hpp"
#include "vector.hpp"
#include "logger.hpp"

Player::Player() : GameObject()
{
    Bound = {30 * TEXTURE_SCALE, 23 * TEXTURE_SCALE};
    Position = {WORLDSIZE_W / 2, WORLDSIZE_H / 2};
}

void Player::Update(GameState &state)
{
    auto input = state.Input;

    Vector2D keyVector = WASDToMovementVector(input.Up, input.Down, input.Left, input.Right);

    if (keyVector.GetMagnitude() > 0.001)
    {
        Rotation = keyVector.GetAngle();
    }

    // Rotation = Vector2D(input.Cursor - Position).GetAngle();

    RightAnimation->Update();
    LeftAnimation->Update();
    UpAnimation->Update();
    DownAnimation->Update();
}

void Player::Render(SDLRenderer &renderer)
{
    if (CloseEnough(Rotation, 0.0))
    {
        RightAnimation->Render(renderer, Position.x - Bound.w / 2, Position.y - Bound.h / 2);
    }
    else if (CloseEnough(Rotation, Pi / 2))
    {
        UpAnimation->Render(renderer, Position.x - Bound.w / 2, Position.y - Bound.h / 2);
    }
    else if (CloseEnough(Rotation, Pi))
    {
        LeftAnimation->Render(renderer, Position.x - Bound.w / 2, Position.y - Bound.h / 2);
    }
    else if (CloseEnough(Rotation, -Pi / 2))
    {
        DownAnimation->Render(renderer, Position.x - Bound.w / 2, Position.y - Bound.h / 2);
    }
}

void Player::SetRightAnimation(std::shared_ptr<Animation> animation)
{
    RightAnimation = animation;
}

void Player::SetLeftAnimation(std::shared_ptr<Animation> animation)
{
    LeftAnimation = animation;
}

void Player::SetUpAnimation(std::shared_ptr<Animation> animation)
{
    UpAnimation = animation;
}

void Player::SetDownAnimation(std::shared_ptr<Animation> animation)
{
    DownAnimation = animation;
}

float Player::GetRotation()
{
    return Rotation;
}
