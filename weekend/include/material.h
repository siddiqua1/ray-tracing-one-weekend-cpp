#ifndef WEEKEND_MATERIAL_H
#define WEEKEND_MATERIAL_H

#include "color.h"
#include "ray.h"

struct hit_record;

class material
{
public:
  virtual ~material() = default;
  virtual bool scatter(
      const ray &r_in, const hit_record &record, color &attenuation, ray &scattered) const
  {
    return false;
  }
};

#include "hittable.h"

class lambertian final : public material
{
public:
  lambertian(const color &albedo) : m_albedo(albedo) {}

  bool scatter(const ray &r_in, const hit_record &record, color &attenuation, ray &scattered) const override
  {
    auto scatter_direction = record.normal + random_unit_vector();

    // intercept bad scattered attempt
    if (scatter_direction.near_zero())
    {
      scatter_direction = record.normal;
    }

    scattered = ray(record.p, scatter_direction);
    attenuation = m_albedo;
    return true;
  }

private:
  color m_albedo;
};

class metal : public material
{
public:
  metal(const color &albedo, f64 fuzz = 0) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

  bool scatter(const ray &r_in, const hit_record &record, color &attenuation, ray &scattered) const override
  {
    vec3 reflected = reflect(r_in.direction(), record.normal);
    reflected = reflected.unit() + (m_fuzz * random_unit_vector());
    scattered = ray(record.p, reflected);
    attenuation = m_albedo;
    return (dot(scattered.direction(), record.normal) > 0);
  }

private:
  color m_albedo;
  f64 m_fuzz;
};

#endif