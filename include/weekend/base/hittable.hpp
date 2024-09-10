#pragma once

#include <optional>
#include <memory>
#include "weekend/core/ray.hpp"
#include "weekend/core/interval.hpp"

namespace weekend::base
{

  class material;

  struct hit_record
  {
    core::point3 p;
    core::vec3 normal;
    double t;
    std::shared_ptr<material> mat;
    bool front_face;

    void set_face_normal(const core::ray &r, const core::vec3 &outward_normal)
    {
      front_face = dot(r.direction(), outward_normal) < 0;
      // (true) 1 -> 2 * 1 - 1 -> 1
      // (false) 0 -> 2 * 0 - 1 -> -1
      normal = outward_normal * (2 * front_face - 1);
    }
  };

  class hittable
  {
  public:
    virtual ~hittable() = default;
    virtual std::optional<hit_record> hit(const core::ray &r, core::interval ray_t) const = 0;
  };

}