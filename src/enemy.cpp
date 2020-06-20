#include "enemy.hpp"

Enemy::Enemy(Point position) : GameObject()
{
    Position = position;
    Bound = {ENEMY_SIZE * TEXTURE_SCALE, ENEMY_SIZE * TEXTURE_SCALE};
}

void Enemy::Update(GameState &state)
{
    if (!Active)
    {
        return;
    }

    Point center = {WORLDSIZE_W / 2, WORLDSIZE_H / 2};

    auto vectorToPlayer = Vector2D(center - Position).Normalize();

    // Chase the player
    Velocity = vectorToPlayer.Scale(MoveSpeed);
    Position = Position + Velocity.GetPoint();
}

void Enemy::Render(SDLRenderer &renderer)
{
    if (!Active)
    {
        return;
    }

    renderer.RenderWholeTexture(MainTexture, GetHitBox());
}

void Enemy::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}
