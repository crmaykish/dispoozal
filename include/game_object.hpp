#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_geometry.hpp"
#include "renderer.hpp"
#include "game_state.hpp"
#include "vector.hpp"

class GameObject
{
protected:
    bool Active = true;
    Point Position; // Center position
    Size Bound;
    Vector2D Velocity;

    int MaxHP = 0;
    int HP = 0;

    GameObject();

public:
    virtual void Update(GameState &state);
    virtual void Render(SDLRenderer &renderer);
    Rectangle GetHitBox();

    bool IsActive();
    void Deactivate();

    Point GetPosition();

    Vector2D GetVelocity();
    void SetVelocity(Vector2D velocity);

    void Damage(int damage);
    bool IsAlive();
};

#endif // GAME_OBJECT_H
