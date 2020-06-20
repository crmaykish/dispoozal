#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.hpp"
#include "game_timer.hpp"
#include "animation.hpp"

class Player : public GameObject
{
protected:
    float Rotation = 0.0;
    std::shared_ptr<Animation> RightAnimation;
    std::shared_ptr<Animation> LeftAnimation;
    std::shared_ptr<Animation> UpAnimation;
    std::shared_ptr<Animation> DownAnimation;

public:
    Player();
    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;
    
    void SetRightAnimation(std::shared_ptr<Animation> animation);
    void SetLeftAnimation(std::shared_ptr<Animation> animation);
    void SetUpAnimation(std::shared_ptr<Animation> animation);
    void SetDownAnimation(std::shared_ptr<Animation> animation);

    float GetRotation();
};

#endif // PLAYER_H
