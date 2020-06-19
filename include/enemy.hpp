#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.hpp"
#include "game_timer.hpp"
#include "texture.hpp"
#include "projectile.hpp"

class Enemy : public GameObject
{
protected:
    std::shared_ptr<Texture> MainTexture;
    float MoveSpeed = 1.0;
    float ProjectileSpeed = 10.0;

    GameTimer FireTimer;

    std::vector<std::shared_ptr<Projectile>> &Projectiles;

public:
    Enemy(Point position, std::vector<std::shared_ptr<Projectile>> &projectiles);

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
};

#endif // ENEMY_H
