#include "core/vec3.hpp"
#include <iostream>

/**
 * Dumping helper function for printing here to optionally include in case i dont wanna bloat a given binary
 */

namespace weekend::utils
{

  inline std::ostream &operator<<(std::ostream &out, const core::vec3 &v)
  {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
  }
}