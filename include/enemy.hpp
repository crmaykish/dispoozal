#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.hpp"
#include "game_timer.hpp"
#include "animation.hpp"

const int ENEMY_SIZE = 28;

enum EnemyDirection
{
    ENEMY_DIR_UNKNOWN,
    ENEMY_UP,
    ENEMY_DOWN,
    ENEMY_LEFT,
    ENEMY_RIGHT
};

class Enemy : public GameObject
{
protected:
    float MoveSpeed = 8.0;
    std::shared_ptr<Animation> MainAnimation;

public:
    EnemyDirection Direction;

    Enemy(Point position, std::shared_ptr<Animation> animation);

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;
};

#endif // ENEMY_H
