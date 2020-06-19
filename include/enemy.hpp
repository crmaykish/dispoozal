#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.hpp"
#include "texture.hpp"

class Enemy : public GameObject
{
protected:
    std::shared_ptr<Texture> MainTexture;
    float MoveSpeed = 1.0;

public:
    Enemy(Point position);

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
};

#endif // ENEMY_H
