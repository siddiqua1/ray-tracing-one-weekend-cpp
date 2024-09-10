#include "weekend/core/random.hpp"
#include "XoshiroCpp.hpp"
#include <random>

namespace weekend::core
{
  f64 random_f64()
  {
    using namespace XoshiroCpp;

    const std::uint64_t seed = 12345;

    static Xoshiro256PlusPlus rng(seed);
    static std::uniform_real_distribution<f64> distribution(0.0, 1.0);
    return distribution(rng);
  }

  f64 random_f64(f64 min, f64 max)
  {
    return min + (max - min) * random_f64();
  }
} // namespace weekend::core