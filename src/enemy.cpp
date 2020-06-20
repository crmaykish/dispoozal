#include "enemy.hpp"

Enemy::Enemy(Point position, std::shared_ptr<Animation> animation) : GameObject()
{
    Position = position;
    Bound = {ENEMY_SIZE * TEXTURE_SCALE, ENEMY_SIZE * TEXTURE_SCALE};

    MainAnimation = animation;
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

    MainAnimation->Update();
}

void Enemy::Render(SDLRenderer &renderer)
{
    if (!Active)
    {
        return;
    }

    MainAnimation->Render(renderer, Position.x - Bound.w / 2, Position.y - Bound.h / 2);
}
