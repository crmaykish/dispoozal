#include "game_object.hpp"

GameObject::GameObject()
{
    Position = {0, 0};
    Bound = {0, 0};
    Velocity = Vector2D(0, 0);
}

void GameObject::Update(GameState &state)
{
}

void GameObject::Render(SDLRenderer &renderer)
{
    renderer.RenderRectangle(GetHitBox(), FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.a);
}

Rectangle GameObject::GetHitBox()
{
    return {
        {Position.x - Bound.w / 2,
         Position.y - Bound.h / 2},
        Bound};
}

bool GameObject::IsActive()
{
    return Active;
}

void GameObject::Deactivate()
{
    Active = false;
}

Point GameObject::GetPosition()
{
    return Position;
}

Vector2D GameObject::GetVelocity()
{
    return Velocity;
}

void GameObject::SetVelocity(Vector2D velocity)
{
    Velocity = velocity;
}

void GameObject::Damage(int damage)
{
    HP -= damage;

    if (HP < 0)
    {
        HP = 0;
    }
}

bool GameObject::IsAlive()
{
    return HP > 0;
}