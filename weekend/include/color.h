#ifndef WEEKEND_COLOR_H
#define WEEKEND_COLOR_H

#include "vec3.h"

#include <iostream>
#include <cassert>

using color = vec3;

void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    /** TODO: check if runtime check makes sense or if we should delegate this out to another type? */
    assert(r >= 0);
    assert(g >= 0);
    assert(b >= 0);

    // Translate the [0,1] component values to the byte range [0,255].
    // These always round DOWN
    int red_byte = int(255.999 * r);
    int green_byte = int(255.999 * g);
    int blue_byte = int(255.999 * b);

    // Write out the pixel color components.
    out << red_byte << ' ' << green_byte << ' ' << blue_byte << '\n';
}

#endif