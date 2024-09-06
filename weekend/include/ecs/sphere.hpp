#pragma once

#include "prelude.hpp"

_RAYTRACE_ECS_START

struct sphere
{
  point3 m_center;
  f64 m_radius;
  f64 m_radius_squared;

  sphere(const point3 &center, f64 radius) : m_center(center), m_radius(radius)
  {
    m_radius_squared = m_radius * m_radius;
  }
};

_RAYTRACE_ECS_END