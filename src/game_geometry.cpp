#include <math.h>
#include "settings.hpp"
#include "game_geometry.hpp"

Point MapToUnitCircle(Point p)
{
    Point r;

    r.x = p.x * std::sqrt(1 - (std::pow(p.y, 2) / 2));
    r.y = p.y * std::sqrt(1 - (std::pow(p.x, 2) / 2));

    return r;
}

bool PointInRect(Point p, Rectangle r)
{
    return (p.x > r.pos.x && p.x < r.pos.x + r.size.w &&
            p.y > r.pos.y && p.y < r.pos.y + r.size.h);
}

bool RectangleCollision(Rectangle a, Rectangle b)
{
    return a.pos.x < b.pos.x + b.size.w &&
           a.pos.x + a.size.w > b.pos.x &&
           a.pos.y < b.pos.y + b.size.h &&
           a.pos.y + a.size.h > b.pos.y;
}
