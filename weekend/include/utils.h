#ifndef WEEKEND_UTILS_H
#define WEEKEND_UTILS_H

#include <iostream>
#include <cmath>
#include "types.h"
// Utility Functions

inline f64 degrees_to_radians(f64 degrees)
{
    return degrees * pi / 180.0;
}

inline f64 random_f64()
{
    return std::rand() / (RAND_MAX + 1.0);
}

inline f64 random_f64(f64 min, f64 max)
{
    return min + (max - min) * random_f64();
}

#endif