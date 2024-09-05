#include "prelude.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++)
  {
    for (int b = -11; b < 11; b++)
    {
      auto choose_mat = random_f64();
      point3 center(a + 0.9 * random_f64(), 0.2, b + 0.9 * random_f64());

      if ((center - point3(4, 0.2, 0)).len() > 0.9)
      {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8)
        {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else if (choose_mat < 0.95)
        {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_f64(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else
        {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  camera cam;

  cam.m_aspect_ratio = 16.0 / 9.0;
  cam.m_image_width = 1200;
  cam.m_samples_per_pixel = 20;
  cam.m_max_depth = 50;

  cam.m_vertical_fov = 20;
  cam.m_look_from = point3(13, 2, 3);
  cam.m_look_at = point3(0, 0, 0);
  cam.m_relative_up = vec3(0, 1, 0);

  cam.m_defocus_angle = 0.6;
  cam.m_focus_dist = 10.0;

  cam.render(world, "image.ppm");
}