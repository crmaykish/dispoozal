#include "projectile.hpp"

Projectile::Projectile(Point position, Vector2D velocity)
{
    Position = position;
    Velocity = velocity;

    Bound = {12, 12};
}

void Projectile::Update(GameState &state)
{
    if (!Active)
    {
        return;
    }

    Position = Position + Velocity.GetPoint();
}

void Projectile::Render(SDLRenderer &renderer)
{
    if (!Active)
    {
        return;
    }
    
    // TODO: add projectile texture
    // renderer.RenderWholeTexture(MainTexture, GetHitBox());
    renderer.RenderRectangle(GetHitBox(), FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.a);
}

void Projectile::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    MainTexture = mainTexture;
}
