#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "game_object.hpp"
#include "texture.hpp"

class Projectile : public GameObject
{
protected:
    std::shared_ptr<Texture> MainTexture;
    int Damage = 10;

public:
    Projectile(Point position, Vector2D velocity);
    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
};

#endif // PROJECTILE_H
