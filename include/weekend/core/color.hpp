#pragma once

#include "interval.hpp"
#include "vec3.hpp"
#include "weekend/utils/file_io.hpp"

namespace weekend::core {

using color = vec3;

inline f64 linear_to_gamma(f64 linear_component) {
  if (linear_component > 0) {
    return std::sqrt(linear_component);
  }
  return 0;
}

inline void write_color(std::ostream &out, const color &pixel_color) {
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

inline void write_color(utils::Write &file, const color &pixel_color) {
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
  file.write_line("{} {} {}", red_byte, green_byte, blue_byte);
}

inline void write_color(std::vector<std::tuple<int, int, int>> &stream,
                        const color &pixel_color) {
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
  stream.push_back(std::tuple{red_byte, green_byte, blue_byte});
}

inline void write_color(std::vector<std::tuple<int, int, int>> &stream,
                        const color &pixel_color, size_t idx) {
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
  stream[idx] = (std::tuple{red_byte, green_byte, blue_byte});
}

} // namespace weekend::core