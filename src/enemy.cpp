#include "enemy.hpp"

Enemy::Enemy(Point position) : FireTimer(300), GameObject()
{
    Position = position;
    Bound = {48, 48};

    FireTimer.Reset();
}

void Enemy::Update(GameState &state)
{
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

    // Update projectiles
    for (auto p : Projectiles)
    {
        p->Update(state);
    }
}

void Enemy::Render(SDLRenderer &renderer)
{
    // Render projectiles
    for (auto p : Projectiles)
    {
        p->Render(renderer);
    }

    renderer.RenderWholeTexture(MainTexture, GetHitBox());
}

void Enemy::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}
