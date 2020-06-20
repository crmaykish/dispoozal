#include "player.hpp"
#include "vector.hpp"
#include "logger.hpp"

Player::Player() :GameObject()
{
    Bound = {8 * TEXTURE_SCALE, 8 * TEXTURE_SCALE};
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
}

void Player::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTextureRotate(MainTexture, GetHitBox(), Rotation * TO_DEGS, {Bound.w / 2, Bound.h / 2});
}

void Player::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}

float Player::GetRotation()
{
    return Rotation;
}
