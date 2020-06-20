#ifndef RAND_UTILS_HPP
#define RAND_UTILS_HPP

// NOTE: All of the methods in this library assume that the random number generator has already been seeded

/**
 * @brief Return true or false at random (50/50)
 */
bool RandomBool();

/**
 * @brief Return a random integer between min and max, inclusive
 */
int RandomInt(int max, int min = 0);

/**
 * @brief Return a random float between min and max, inclusive
 */
float RandomFloat(float max, float min = 0);

#endif // RAND_UTILS_HPP
