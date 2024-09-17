#pragma once

#include "weekend/core/types.hpp"

namespace weekend::utils {

inline f64 degrees_to_radians(f64 degrees) { return degrees * pi / 180.0; }

} // namespace weekend::utils