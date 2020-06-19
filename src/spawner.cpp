#include "spawner.hpp"

Spawner::Spawner(Point position,
                 std::vector<std::shared_ptr<Enemy>> &enemies,
                 std::vector<std::shared_ptr<Projectile>> &projectiles)
    : SpawnTimer(4000), Enemies(enemies), Projectiles(projectiles), GameObject()
{
    Position = position;
    Bound = {128, 128};

    SpawnTimer.Reset();
}

void Spawner::Update(GameState &state)
{
    // Spawn additional enemies
    if (SpawnTimer.IsExpired())
    {
        Point enemyPosition = Position + Point{0, -100};
        auto ne = std::make_shared<Enemy>(enemyPosition, Projectiles);
        ne->SetMainTexture(EnemyTexture);

        Enemies.push_back(ne);

        SpawnTimer.Reset();
    }
}

void Spawner::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTexture(MainTexture, GetHitBox());
}

void Spawner::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}

void Spawner::SetEnemyTexture(std::shared_ptr<Texture> enemyTexture)
{
    EnemyTexture = enemyTexture;
}
