#ifndef GAME_GEOMETRY_H
#define GAME_GEOMETRY_H

#include "math_utils.hpp"
#include <SDL2/SDL.h>

// STRUCTURE DEFINITIONS

struct Point
{
    double x, y;

    Point operator+(const Point &P)
    {
        return {x + P.x, y + P.y};
    }

    Point operator-(const Point &P)
    {
        return {x - P.x, y - P.y};
    }

    void operator+=(const Point &P)
    {
        x += P.x;
        y += P.y;
    }

    void operator=(const Point &P)
    {
        x = P.x;
        y = P.y;
    }
};

struct Size
{
    float w = 0.0;
    float h = 0.0;

    void operator=(const Size &S)
    {
        w = S.w;
        h = S.h;
    }
};

struct Rectangle
{
    Point pos;
    Size size;

    Point Center()
    {
        return {pos.x + size.w / 2, pos.y + size.h / 2};
    }

    void operator=(const Rectangle &R)
    {
        pos = R.pos;
        size = R.size;
    }
};

// DATA CONVERSIONS

Point MapToUnitCircle(Point p);
bool PointInRect(Point p, Rectangle r);
bool RectangleCollision(Rectangle a, Rectangle b);

// PROCESSING

// Point NextPosition(Point *p, Vector *v);
// void UpdatePosition(Point *p, Vector *v);

#endif // GAME_GEOMETRY_H
