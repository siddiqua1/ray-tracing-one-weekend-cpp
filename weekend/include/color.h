#ifndef WEEKEND_COLOR_H
#define WEEKEND_COLOR_H

#include "vec3.h"
#include "interval.h"

using color = vec3;

inline f64 linear_to_gamma(f64 linear_component)
{
    if (linear_component > 0)
    {
        return std::sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    // These always round DOWN
    static const interval intensity(0.000, 0.999);
    int red_byte = int(256 * intensity.clamp(r));
    int green_byte = int(256 * intensity.clamp(g));
    int blue_byte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << red_byte << ' ' << green_byte << ' ' << blue_byte << '\n';
}

#endif