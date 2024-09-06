#include "core/vec3.hpp"

using namespace weekend::core;

vec3::vec3() : m_repr{0, 0, 0} {}

vec3::vec3(const f64 x, const f64 y, const f64 z) : m_repr{x, y, z} {}

inline f64 vec3::x() const { return m_repr[0]; }
inline f64 vec3::y() const { return m_repr[1]; }
inline f64 vec3::z() const { return m_repr[2]; }

f64 vec3::operator[](const size_t i) const { return m_repr[i]; }
f64 &vec3::operator[](const size_t i) { return m_repr[i]; }

inline vec3 vec3::operator-() const { return vec3(-x(), -y(), -z()); }
