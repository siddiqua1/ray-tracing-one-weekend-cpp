#pragma once
#include "weekend/base/hittable.hpp"
#include "weekend/base/material.hpp"
#include "weekend/core/types.hpp"
#include "weekend/utils/file_io.hpp"
#include "weekend/utils/radians.hpp"


#include <cassert>
#include <iostream>
#include <thread>

namespace weekend {
namespace base {

class camera {
public:
  f64 m_aspect_ratio{1.0};
  i32 m_image_width{100};
  i32 m_samples_per_pixel{10};
  i32 m_max_depth{10};
  f64 m_reflective_rate{0.5};
  core::point3 m_look_from = core::point3::zero();
  core::point3 m_look_at = core::point3(0, 0, -1);
  core::vec3 m_relative_up = core::vec3(0, 1, 0);

  f64 m_vertical_fov{90};

  f64 m_defocus_angle = 0;
  f64 m_focus_dist = 10;

  void initialize() {
    m_image_height = int(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    m_center = m_look_from;

    auto theta = utils::degrees_to_radians(m_vertical_fov);
    auto h = std::tan(theta / 2);

    m_viewport_height = 2 * h * m_focus_dist;

    m_viewport_width =
        m_viewport_height * (f64(m_image_width) / m_image_height);

    m_basis_w = (m_look_from - m_look_at).unit();
    m_basis_u = cross(m_relative_up, m_basis_w).unit();
    m_basis_v = cross(m_basis_w, m_basis_u);

    m_viewport_u = m_viewport_width * m_basis_u;
    m_viewport_v = -m_viewport_height * m_basis_v;

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    m_viewport_upper_left = m_center - (m_focus_dist * m_basis_w) -
                            m_viewport_u / 2 - m_viewport_v / 2;

    m_pixel00_loc =
        m_viewport_upper_left + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    m_pixel_samples_scale = 1.0 / m_samples_per_pixel;

    auto defocus_radius =
        m_focus_dist * std::tan(utils::degrees_to_radians(m_defocus_angle / 2));
    m_defocus_disk_u = m_basis_u * defocus_radius;
    m_defocus_disk_v = m_basis_v * defocus_radius;
  }

  inline core::vec3 pixel_ij(i32 i, i32 j, const core::vec3 &offset) const {
    return m_pixel00_loc + ((i + offset.x()) * m_pixel_delta_u) +
           ((j + offset.y()) * m_pixel_delta_v);
  }

  void render(const hittable &world) {
    initialize();
    std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

    for (i32 j = 0; j < m_image_height; j++) {
      std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' '
                << std::flush;
      for (i32 i = 0; i < m_image_width; i++) {
        core::color pixel_color(0, 0, 0);
        for (i32 sample = 0; sample < m_samples_per_pixel; ++sample) {
          core::ray r = get_ray(i, j);
          pixel_color += ray_color(r, m_max_depth, world);
        }
        write_color(std::cout, m_pixel_samples_scale * pixel_color);
      }
    }
    std::clog << "\rDone.                 \n";
  }

  void render(const hittable &world, const char *file_name) {
    initialize();
    auto file = utils::open<'w'>(file_name);

    file.write_line("P3");
    file.write_line("{} {}", m_image_width, m_image_height);
    file.write_line("255");

    const i32 ITERATIONS = m_image_height * m_image_width;

    std::vector<std::tuple<int, int, int>> color_buffer(ITERATIONS);

    for (i32 j = 0; j < m_image_height; j++) {
      std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' '
                << std::flush;

      assert(m_image_width % m_THREAD_COUNT == 0);
      for (i32 i = 0; i < m_image_width; i += m_THREAD_COUNT) {

        auto thread_routine = [this, &world, &color_buffer](i32 kj, i32 ki) {
          size_t insertion_idx = kj * m_image_width + ki;
          core::color pixel_color(0, 0, 0);

          for (i32 sample = 0; sample < m_samples_per_pixel; ++sample) {
            core::ray r = get_ray(ki, kj);
            pixel_color += ray_color(r, m_max_depth, world);
          }
          write_color(color_buffer, m_pixel_samples_scale * pixel_color,
                      insertion_idx);
        };

        std::thread thread_pool[m_THREAD_COUNT];

        i32 offset = 0;
        for (auto &t : thread_pool) {
          t = std::thread(thread_routine, j, i + offset);
          offset++;
        }
        for (auto &t : thread_pool) {
          t.join();
        }
      }
    }

    const size_t len = color_buffer.size();
    for (size_t i = 0; i < len; ++i) {
      const auto &[r, g, b] = color_buffer[i];
      file.write_line("{} {} {}", r, g, b);
    }
    std::clog << "\rDone.                 \n";
  }

private:
  core::point3 m_center{core::point3::zero()};

  i32 m_image_height{0};
  f64 m_viewport_width{0};
  f64 m_viewport_height{2.0};

  core::vec3 m_viewport_u{};
  core::vec3 m_viewport_v{};

  core::vec3 m_pixel_delta_u{};
  core::vec3 m_pixel_delta_v{};

  core::vec3 m_viewport_upper_left{};
  core::vec3 m_pixel00_loc{};

  f64 m_pixel_samples_scale;

  core::vec3 m_basis_u, m_basis_v, m_basis_w;

  core::vec3 m_defocus_disk_u;
  core::vec3 m_defocus_disk_v;

  static const size_t m_THREAD_COUNT = 8;

private:
  core::color background_gradient(const core::ray &r) const {
    core::vec3 unit_direction = r.direction().unit();

    // y which is [-1.0, 1.0]
    // transform to a which is [0.0, 1.0]
    auto a = 0.5 * (unit_direction.y() + 1.0);
    const core::color start = core::color(1.0, 1.0, 1.0);
    const core::color end = core::color(0.5, 0.7, 1.0);
    return (1.0 - a) * start + a * end;
  }

  core::color ray_color(const core::ray &r, i32 depth,
                        const base::hittable &world) const {
    if (depth <= 0) {
      return core::color(0, 0, 0);
    }
    const auto sphere_center = core::point3(0, 0, -1);
    const auto sphere_radius = 0.5;

    static const f64 MINIMUM_ORIGIN_DELTA = 0.001;
    auto maybe_record =
        world.hit(r, core::interval(MINIMUM_ORIGIN_DELTA, infinity));

    if (maybe_record.has_value()) {
      auto record = maybe_record.value();
      core::ray scattered;
      core::color attenuation;

      if (record.mat->scatter(r, record, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
      return core::color(0, 0, 0);
    }
    return background_gradient(r);
  }

  core::vec3 sample_square() const {
    return core::vec3(core::random_f64() - 0.5, core::random_f64() - 0.5, 0);
  }

  core::ray get_ray(i32 i, i32 j) const {
    auto offset = sample_square();
    auto pixel_sample = pixel_ij(i, j, offset);
    auto ray_origin = (m_defocus_angle <= 0) ? m_center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return core::ray(ray_origin, ray_direction);
  }

  core::point3 defocus_disk_sample() const {
    // Returns a random point in the camera defocus disk.
    auto p = core::random_in_unit_disk();
    return m_center + (p[0] * m_defocus_disk_u) + (p[1] * m_defocus_disk_v);
  }
};

} // namespace base
} // namespace weekend