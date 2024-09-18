#include <weekend/base/camera.hpp>
#include <weekend/base/hittable.hpp>
#include <weekend/base/hittable_list.hpp>
#include <weekend/base/material.hpp>
#include <weekend/base/sphere.hpp>

using namespace weekend;
using weekend::base::camera;
using weekend::base::dielectric;
using weekend::base::hittable;
using weekend::base::hittable_list;
using weekend::base::lambertian;
using weekend::base::metal;
using weekend::base::sphere;
using weekend::core::color;
using weekend::core::interval;
using weekend::core::point3;
using weekend::core::ray;
using weekend::core::vec3;

using std::make_shared;

int main() {
  hittable_list world;

  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.50);
  auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  camera cam;

  cam.m_aspect_ratio = 16.0 / 9.0;
  cam.m_image_width = 400;
  cam.m_samples_per_pixel = 100;
  cam.m_max_depth = 50;

  cam.m_vertical_fov = 20;
  cam.m_look_from = point3(-2, 2, 1);
  cam.m_look_at = point3(0, 0, -1);
  cam.m_relative_up = vec3(0, 1, 0);

  cam.render(world, "dielectric_alt_view.ppm");
}