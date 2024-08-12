#include "prelude.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include "material.h"

struct camera
{
    point3 m_center{point3::zero()};

    f64 m_aspect_ratio{0};
    i32 m_image_width{0};
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

    camera(f64 aspect_ratio, i32 width) : m_aspect_ratio(aspect_ratio), m_image_width(width)
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
    }

    inline vec3 pixel_ij(i32 i, i32 j)
    {
        return m_pixel00_loc + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
    }
};

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

color ray_color(const ray &r, const hittable &world)
{
    const auto sphere_center = point3(0, 0, -1);
    const auto sphere_radius = 0.5;

    auto maybe_record = world.hit(r, interval(0, infinity));

    if (maybe_record.has_value())
    {
        auto record = maybe_record.value();
        return 0.5 * (record.normal + color(1, 1, 1));
    }
    return background_gradient(r);
}

int main()
{
    // Image

    const i32 width = 400;
    const f64 aspect = 16.0 / 9.0;
    camera cam = camera(aspect, width);
    // Render
    auto default_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, default_material));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, default_material));

    std::cout
        << "P3\n"
        << cam.m_image_width << ' ' << cam.m_image_height << "\n255\n";

    for (i32 j = 0; j < cam.m_image_height; j++)
    {
        std::clog << "\rScanlines remaining: "
                  << (cam.m_image_height - j) << ' ' << std::flush;
        for (i32 i = 0; i < cam.m_image_width; i++)
        {
            auto pixel_ij_center = cam.pixel_ij(i, j);
            auto ray_direction = pixel_ij_center - cam.m_center;

            ray r(cam.m_center, ray_direction);
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}