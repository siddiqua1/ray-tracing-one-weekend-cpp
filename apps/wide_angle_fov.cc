#include "prelude.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
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