#include "enemy.hpp"

Enemy::Enemy(Point position) : GameObject()
{
    Position = position;
    Bound = {48, 48};
}

void Enemy::Update(GameState &state)
{
    // Chase the player
    Velocity = Vector2D(state.GetPlayerPosition() - Position).Normalize().Scale(MoveSpeed);
    Position = Position + Velocity.GetPoint();
}

void Enemy::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTexture(MainTexture, GetHitBox());
}

void Enemy::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}
