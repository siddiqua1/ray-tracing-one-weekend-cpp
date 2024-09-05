#ifndef WEEKEND_UTILS_H
#define WEEKEND_UTILS_H

#include <random>
#include <iostream>
#include <cmath>
#include "types.h"
#include "XoshiroCpp.hpp"
// Utility Functions

inline f64 degrees_to_radians(f64 degrees)
{
    return degrees * pi / 180.0;
}

inline f64 random_f64()
{
    using namespace XoshiroCpp;

    const std::uint64_t seed = 12345;

    static Xoshiro256PlusPlus rng(seed);
    static std::uniform_real_distribution<f64> distribution(0.0, 1.0);
    return distribution(rng);
}

inline f64 random_f64(f64 min, f64 max)
{
    return min + (max - min) * random_f64();
}

#endif