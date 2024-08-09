#ifndef WEEKEND_UTILS_H
#define WEEKEND_UTILS_H

#include <iostream>
#include "vec3.h"

/**
 * Dumping helper function for printing here to optionally include in case i dont wanna bloat a given binary
 */

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

#endif