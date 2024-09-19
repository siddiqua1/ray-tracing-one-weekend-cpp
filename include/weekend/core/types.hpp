#pragma once

#include <limits>
#include <stdint.h>

namespace weekend {

typedef double f64;
typedef float f32;

typedef int32_t i32;
typedef int64_t i64;

static_assert(sizeof(i32) == sizeof(f32));
static_assert(sizeof(i64) == sizeof(f64));

// Constants
const f64 infinity = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

} // namespace weekend