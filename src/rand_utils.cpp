#include <stdlib.h>
#include "rand_utils.hpp"

bool RandomBool()
{
    return (rand() % 2 == 0);
}

int RandomInt(int max, int min)
{
    return min + (rand() % (max - min));
}

float RandomFloat(float max, float min)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}