#include <iostream>
#include "types.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

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

color ray_color(const ray &r)
{

    vec3 unit_direction = r.direction().unit();

    // y which is [-1.0, 1.0]
    // transform to a which is [0.0, 1.0]
    auto a = 0.5 * (unit_direction.y() + 1.0);
    const color start = color(1.0, 1.0, 1.0);
    const color end = color(0.5, 0.7, 1.0);
    return (1.0 - a) * start + a * end;
}

int main()
{
    // Image

    const i32 width = 400;
    const f64 aspect = 16.0 / 9.0;
    camera cam = camera(aspect, width);
    // Render

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
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}