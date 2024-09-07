#pragma once

#include "types.hpp"
namespace weekend::core {
class interval {
public:
  f64 m_min, m_max;

  interval() : m_min(+infinity), m_max(-infinity) {}
  interval(f64 min, f64 max) : m_min(min), m_max(max) {}

  f64 size() const { return m_max - m_min; }

  bool contains(f64 x) const { return m_min <= x && x <= m_max; }

  bool surrounds(f64 x) const { return m_min < x && x < m_max; }

  f64 clamp(f64 x) const {
    if (x < m_min) {
      return m_min;
    }
    if (x > m_max) {
      return m_max;
    }
    return x;
  }

  static const interval empty, universe;
};

inline const interval interval::empty = interval(+infinity, -infinity);
inline const interval interval::universe = interval(-infinity, +infinity);

} // namespace weekend::core
