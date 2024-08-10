#ifndef WEEKEND_SPHERE_H
#define WEEKEND_SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere final : public hittable
{
public:
  sphere(const point3 &center, f64 radius) : m_center(center), m_radius(std::fmax(0, radius)), m_radius_squared(m_radius * m_radius)
  {
  }

  std::optional<hit_record> hit(const ray &r, interval ray_t) const override
  {
    const auto C_minus_Q = m_center - r.origin();
    const auto &dir = r.direction();
    const auto a = dir.len_squared();
    const auto h = dot(dir, C_minus_Q);
    const auto c = C_minus_Q.len_squared() - m_radius_squared;
    const auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
      return std::nullopt;
    }

    const auto sqrt_discriminant = std::sqrt(discriminant);

    auto root = (h - sqrt_discriminant) / a;
    if (!ray_t.surrounds(root))
    {
      root = (h + sqrt_discriminant) / a;
      if (!ray_t.surrounds(root))
      {
        return std::nullopt;
      }
    }

    hit_record result{};
    result.t = root;
    result.p = r.at(result.t);
    const auto outward_normal = (result.p - m_center) / m_radius;
    result.set_face_normal(r, outward_normal);
    return result;
  }

private:
  point3 m_center;
  f64 m_radius;
  f64 m_radius_squared;
};

#endif