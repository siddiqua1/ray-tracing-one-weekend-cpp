#pragma once

namespace utils::enums
{

  template <typename Enum>
  bool valid(Enum e)
  {
    return static_cast<int>(Enum::INVALID) < static_cast<int>(e) && static_cast<int>(e) < static_cast<int>(Enum::COUNT);
  }

}