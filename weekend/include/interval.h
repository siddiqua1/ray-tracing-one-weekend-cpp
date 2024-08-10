#ifndef WEEKEND_INTERVAL_H
#define WEEKEND_INTERVAL_H

#include "prelude.h"

class interval
{
public:
  f64 m_min, m_max;

  interval() : m_min(+infinity), m_max(-infinity) {}
  interval(f64 min, f64 max) : m_min(min), m_max(max) {}

  f64 size() const
  {
    return m_max - m_min;
  }

  bool contains(f64 x) const
  {
    return m_min <= x && x <= m_max;
  }

  bool surrounds(f64 x) const
  {
    return m_min < x && x < m_max;
  }

  static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif