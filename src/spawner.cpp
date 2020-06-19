#include "spawner.hpp"

Spawner::Spawner(Point position) : SpawnTimer(1000), GameObject()
{
    Position = position;
    Bound = {128, 128};

    SpawnTimer.Reset();
}

void Spawner::Update(GameState &state)
{
    // Update enemies under spawner control
    for (auto e : Enemies)
    {
        e->Update(state);
    }

    // Spawn additional enemies
    if (SpawnTimer.IsExpired())
    {
        Point enemyPosition = Position + Point{0, -100};
        auto ne = std::make_shared<Enemy>(enemyPosition);
        ne->SetMainTexture(EnemyTexture);

        Enemies.push_back(ne);

        SpawnTimer.Reset();
    }
}

void Spawner::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTexture(MainTexture, GetHitBox());

    for (auto e : Enemies)
    {
        e->Render(renderer);
    }
}

void Spawner::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}

void Spawner::SetEnemyTexture(std::shared_ptr<Texture> enemyTexture)
{
    EnemyTexture = enemyTexture;
}
