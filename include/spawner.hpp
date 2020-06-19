#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>
#include "game_object.hpp"
#include "game_timer.hpp"
#include "enemy.hpp"

class Spawner : public GameObject
{
protected:
    std::shared_ptr<Texture> MainTexture;
    std::shared_ptr<Texture> EnemyTexture;
    std::vector<std::shared_ptr<Enemy>> Enemies;

    GameTimer SpawnTimer;

public:
    Spawner(Point position);

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    void SetEnemyTexture(std::shared_ptr<Texture> enemyTexture);
};

#endif // SPAWNER_H
