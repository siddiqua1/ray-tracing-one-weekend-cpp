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

class dielectric : public material
{
public:
  dielectric(f64 refraction_index) : m_refraction_index(refraction_index) {}

  bool scatter(const ray &r_in, const hit_record &record, color &attentuation, ray &scattered) const override
  {

    attentuation = color(1.0, 1.0, 1.0);
    double ri = record.front_face ? (1 / m_refraction_index) : m_refraction_index;

    vec3 unit_direction = r_in.direction().unit();

    f64 cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
    f64 sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || schlick_approx(cos_theta, ri) > random_f64())
    {
      direction = reflect(unit_direction, record.normal);
    }
    else
    {
      direction = refract(unit_direction, record.normal, ri);
    }

    scattered = ray(record.p, direction);
    return true;
  }

private:
  f64 m_refraction_index;

  static f64 schlick_approx(f64 cosine, f64 refraction_index)
  {
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }
};

#endif