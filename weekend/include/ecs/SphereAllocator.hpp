#pragma once

#include "prelude.hpp"
#include "sphere.hpp"

#include <vector>

_RAYTRACE_ECS_START

class SphereAllocator
{

private:
  std::vector<point3> m_centers;
  std::vector<f64> m_radii;
};

_RAYTRACE_ECS_END