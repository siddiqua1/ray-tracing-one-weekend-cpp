#include "weekend/core/vec3.hpp"

using namespace weekend::core;
using namespace weekend;

vec3::vec3() : m_repr{0, 0, 0} {}

vec3::vec3(const f64 x, const f64 y, const f64 z) : m_repr{x, y, z} {}

f64 vec3::operator[](const size_t i) const { return m_repr[i]; }
f64 &vec3::operator[](const size_t i) { return m_repr[i]; }

