#include <iostream>
#include <weekend/core/color.hpp>
#include <weekend/core/vec3.hpp>
#include <weekend/utils/file_io.hpp>

int main() {
  // Image

  int image_width = 256;
  int image_height = 256;

  // Render
  auto file = weekend::utils::open<'w'>("output_ppm.ppm");
  file.write_line("P3");
  file.write_line("{} {}", image_width, image_height);
  file.write_line("255");

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
              << std::flush;
    for (int i = 0; i < image_width; i++) {
      auto pixel_color = weekend::core::color(
          double(i) / (image_width - 1), double(j) / (image_height - 1), 0);

      const auto [r, g, b] = weekend::core::to_tuple(pixel_color);
      file.write_line("{} {} {}", r, g, b);
    }
  }
  std::clog << "\rDone.                 \n";
}