#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.hpp"
#include "game_timer.hpp"
#include "texture.hpp"

class Player : public GameObject
{
protected:
    float Rotation = 0.0;
    std::shared_ptr<Texture> MainTexture;

public:
    Player();
    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;
    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    float GetRotation();
};

#endif // PLAYER_H
