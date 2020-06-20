#include "enemy.hpp"

Enemy::Enemy(Point position)
    : FireTimer(1000), GameObject()
{
    Position = position;
    Bound = {8 * TEXTURE_SCALE, 8 * TEXTURE_SCALE};

    FireTimer.Reset();

    MaxHP = 30;
    HP = MaxHP;
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

    // Constantly fire at the player
    // if (FireTimer.IsExpired())
    // {
    //     Vector2D projectileVelocity = vectorToPlayer.Scale(ProjectileSpeed);
    //     auto projectile = std::make_shared<Projectile>(Position, projectileVelocity);
    //     Projectiles.push_back(projectile);
    //     FireTimer.Reset();
    // }

    if (!IsAlive())
    {
        Active = false;
    }
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
