#include "player.hpp"
#include "vector.hpp"
#include "logger.hpp"

Player::Player() : FlipTimer(250), GameObject()
{
    Position = {100, 100};
    Bound = {64, 64};

    FlipTimer.Reset();
}

void Player::Update(GameState &state)
{
    auto input = state.GetInput();

    // Movement
    auto moveVector = WASDToMovementVector(input.Up, input.Down, input.Left, input.Right);

    // TODO: add a bit of accel/decel to player movement

    Velocity = moveVector.Scale(MoveSpeed);
    Position = Position + Velocity.GetPoint();

    // Angle
    Rotation = Vector2D(input.Cursor - Position).GetAngle();

    // Flip
    if (input.FireMain && FlipTimer.IsExpired())
    {
        Flip = !Flip;
        FlipTimer.Reset();
    }

    // Update player position in the game state
    state.SetPlayerPosition(Position);
}

void Player::Render(SDLRenderer &renderer)
{
    float angle = Flip ? Rotation + Pi : Rotation;

    renderer.RenderWholeTextureRotate(MainTexture, GetHitBox(), angle * TO_DEGS, {Bound.w / 2, Bound.h / 2});
}

void Player::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}