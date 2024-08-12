#ifndef WEEKEND_CAMERA_H
#define WEEKEND_CAMERA_H

#include "prelude.h"
#include "hittable.h"
#include "material.h"

class camera
{
public:
  f64 m_aspect_ratio{1.0};
  i32 m_image_width{100};
  i32 m_samples_per_pixel{10};
  i32 m_max_depth{10};
  f64 m_reflective_rate{0.5};

  void initialize()
  {
    m_image_height = int(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    m_viewport_width = m_viewport_height * (f64(m_image_width) / m_image_height);

    m_viewport_u = vec3(m_viewport_width, 0, 0);
    m_viewport_v = vec3(0, -m_viewport_height, 0);

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    m_viewport_upper_left = m_center - vec3(0, 0, m_focal_length) - m_viewport_u / 2 - m_viewport_v / 2;

    m_pixel00_loc = m_viewport_upper_left + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    m_pixel_samples_scale = 1.0 / m_samples_per_pixel;
  }

  inline vec3 pixel_ij(i32 i, i32 j, const vec3 &offset) const
  {
    return m_pixel00_loc + ((i + offset.x()) * m_pixel_delta_u) + ((j + offset.y()) * m_pixel_delta_v);
  }

  void render(const hittable &world)
  {
    initialize();
    std::cout
        << "P3\n"
        << m_image_width << ' ' << m_image_height << "\n255\n";

    for (i32 j = 0; j < m_image_height; j++)
    {
      std::clog << "\rScanlines remaining: "
                << (m_image_height - j) << ' ' << std::flush;
      for (i32 i = 0; i < m_image_width; i++)
      {
        color pixel_color(0, 0, 0);
        for (i32 sample = 0; sample < m_samples_per_pixel; ++sample)
        {
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, m_max_depth, world);
        }
        write_color(std::cout, m_pixel_samples_scale * pixel_color);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  point3 m_center{point3::zero()};

  i32 m_image_height{0};
  f64 m_focal_length{1.0};
  f64 m_viewport_width{0};
  f64 m_viewport_height{2.0};

  vec3 m_viewport_u{};
  vec3 m_viewport_v{};

  vec3 m_pixel_delta_u{};
  vec3 m_pixel_delta_v{};

  vec3 m_viewport_upper_left{};
  vec3 m_pixel00_loc{};

  f64 m_pixel_samples_scale;

private:
  color background_gradient(const ray &r)
  {
    vec3 unit_direction = r.direction().unit();

    // y which is [-1.0, 1.0]
    // transform to a which is [0.0, 1.0]
    auto a = 0.5 * (unit_direction.y() + 1.0);
    const color start = color(1.0, 1.0, 1.0);
    const color end = color(0.5, 0.7, 1.0);
    return (1.0 - a) * start + a * end;
  }

  color ray_color(const ray &r, i32 depth, const hittable &world)
  {
    if (depth <= 0)
    {
      return color(0, 0, 0);
    }
    const auto sphere_center = point3(0, 0, -1);
    const auto sphere_radius = 0.5;

    static const f64 MINIMUM_ORIGIN_DELTA = 0.001;
    auto maybe_record = world.hit(r, interval(MINIMUM_ORIGIN_DELTA, infinity));

    if (maybe_record.has_value())
    {
      auto record = maybe_record.value();
      ray scattered;
      color attenuation;

      if (record.mat->scatter(r, record, attenuation, scattered))
      {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
      return color(0, 0, 0);
    }
    return background_gradient(r);
  }

  vec3 sample_square() const
  {
    return vec3(random_f64() - 0.5, random_f64() - 0.5, 0);
  }

  ray get_ray(i32 i, i32 j) const
  {
    auto offset = sample_square();
    auto pixel_sample = pixel_ij(i, j, offset);
    auto ray_origin = m_center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
  }
};

#endif