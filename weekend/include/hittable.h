#ifndef WEEKEND_HITTABLE_H
#define WEEKEND_HITTABLE_H

#include <optional>
#include "ray.h"
#include "interval.h"

struct hit_record
{
  point3 p;
  vec3 normal;
  double t;
  bool front_face;

  void set_face_normal(const ray &r, const vec3 &outward_normal)
  {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable
{
public:
  virtual ~hittable() = default;
  virtual std::optional<hit_record> hit(const ray &r, interval ray_t) const = 0;
};

#endif