#include "vector.hpp"

// Return the angle of a vector [0,0] to p
static float VectorAngle(Point p);

// Return the magnitude of vector from [0,0] to p
static float VectorMagnitude(Point p);

Vector2D::Vector2D() : Magnitude(0.0), Angle(0.0) {}

Vector2D::Vector2D(Vector2D &v)
{
    Magnitude = v.Magnitude;
    Angle = v.Angle;
}

Vector2D::Vector2D(Point point)
    : Magnitude(VectorMagnitude(point)),
      Angle(VectorAngle(point)) {}

Vector2D::Vector2D(float magnitude, float angle)
    : Magnitude(magnitude),
      Angle(angle) {}

Point Vector2D::GetPoint()
{
    return {Magnitude * std::cos(Angle),
            Magnitude * std::sin(Angle)};
}

float Vector2D::GetMagnitude()
{
    return Magnitude;
}

float Vector2D::GetAngle()
{
    return Angle;
}

Vector2D Vector2D::Scale(float scaleFactor)
{
    return Vector2D(Magnitude * scaleFactor, Angle);
}

Vector2D Vector2D::Normalize()
{
    return Vector2D(1.0, Angle);
}

Vector2D Vector2D::Add(Vector2D v)
{
    return Vector2D(GetPoint() + v.GetPoint());
}

Vector2D Vector2D::Rotate(float angle)
{
    return Vector2D(Magnitude, Angle + angle);
}

static float VectorAngle(Point p)
{
    if (p.x >= 0.0)
    {
        return std::atan(p.y / p.x);
    }
    else
    {
        // TODO: I think this is subtracting an extra rotation
        return std::atan(p.y / p.x) - (180.0 * TO_RADS);
    }
}

static float VectorMagnitude(Point p)
{
    return std::sqrt(std::pow(p.x, 2) + std::pow(p.y, 2));
}

Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R)
{
    float magnitude = 1.0;
    float angle = 0.0;

    if ((R && L) || U && D)
    {
        magnitude = 0.0;
    }
    else if (R && U)
    {
        angle = 45.0 * TO_RADS;
    }
    else if (L && U)
    {
        angle = 135.0 * TO_RADS;
    }
    else if (L && D)
    {
        angle = 225.0 * TO_RADS;
    }
    else if (R && D)
    {
        angle = 315.0 * TO_RADS;
    }
    else if (R)
    {
        angle = 0.0 * TO_RADS;
    }
    else if (U)
    {
        angle = 90.0 * TO_RADS;
    }
    else if (L)
    {
        angle = 180.0 * TO_RADS;
    }
    else if (D)
    {
        angle = 270.0 * TO_RADS;
    }
    else
    {
        magnitude = 0.0;
        angle = 0.0;
    }

    return Vector2D(magnitude, angle);
}