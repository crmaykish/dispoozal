#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "game_geometry.hpp"

class Vector2D
{
private:
    float Magnitude = 0.0;
    float Angle = 0.0;

public:
    Vector2D();
    Vector2D(Vector2D &v);
    Vector2D(Point point);
    Vector2D(float magnitude, float angle);
    Point GetPoint();
    float GetMagnitude();
    float GetAngle();

    Vector2D Scale(float scaleFactor);
    Vector2D Normalize();
    Vector2D Add(Vector2D v);
    Vector2D Rotate(float angle);
};

// TODO: move this
/** @brief Convert a combination of directions into a unit vector for movement at that angle */
Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R);

#endif // VECTOR_HPP
