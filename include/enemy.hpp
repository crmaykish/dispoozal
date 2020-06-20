#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.hpp"
#include "game_timer.hpp"
#include "texture.hpp"

const int ENEMY_SIZE = 8;

enum EnemyDirection
{
    ENEMY_UP,
    ENEMY_DOWN,
    ENEMY_LEFT,
    ENEMY_RIGHT
};

class Enemy : public GameObject
{
protected:
    std::shared_ptr<Texture> MainTexture;
    float MoveSpeed = 8.0;

public:
    EnemyDirection Direction;

    Enemy(Point position);

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
};

#endif // ENEMY_H
