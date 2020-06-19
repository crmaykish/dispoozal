#include "enemy.hpp"

Enemy::Enemy(Point position,
             std::vector<std::shared_ptr<Projectile>> &projectiles)
    : FireTimer(1000), Projectiles(projectiles), GameObject()
{
    Position = position;
    Bound = {48, 48};

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

    auto vectorToPlayer = Vector2D(state.GetPlayerPosition() - Position).Normalize();

    // Chase the player
    Velocity = vectorToPlayer.Scale(MoveSpeed);
    Position = Position + Velocity.GetPoint();

    // Constantly fire at the player
    if (FireTimer.IsExpired())
    {
        Vector2D projectileVelocity = vectorToPlayer.Scale(ProjectileSpeed);
        auto projectile = std::make_shared<Projectile>(Position, projectileVelocity);
        Projectiles.push_back(projectile);
        FireTimer.Reset();
    }

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
