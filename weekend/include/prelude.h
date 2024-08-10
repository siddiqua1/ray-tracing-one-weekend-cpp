#ifndef WEEKEND_PRELUDE_H
#define WEEKEND_PRELUDE_H

#include <cmath>
#include <iostream>
#include <optional>
#include <limits>
#include <memory>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

#include "types.h"

// Constants
const f64 infinity = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

// Utility Functions

inline f64 degrees_to_radians(f64 degrees)
{
  return degrees * pi / 180.0;
}

// Common Headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif