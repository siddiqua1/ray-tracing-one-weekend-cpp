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

  auto R = std::cos(pi / 4);

  auto material_left = make_shared<lambertian>(color(0, 0, 1));
  auto material_right = make_shared<lambertian>(color(1, 0, 0));

  world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
  world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

  camera cam;

  cam.m_aspect_ratio = 16.0 / 9.0;
  cam.m_image_width = 400;
  cam.m_samples_per_pixel = 100;
  cam.m_max_depth = 50;

  cam.m_vertical_fov = 90;

  cam.render(world);
}