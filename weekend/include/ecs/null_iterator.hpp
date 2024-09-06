#pragma once

#include "prelude.hpp"

_RAYTRACE_ECS_START

/**
 * @brief Class with no real iteration support. Intended to be slotted in for
 * statically dispathced interface that require iterator support for quicker
 * iterations.
 */
class null_iterator
{
public:
  /**
   * @brief required to support range based for loops
   *
   * @param _rhs other iterator to compare to
   * @return true; this should never occur
   * @return false; always the case as all instances of this iterator are
   * equivalent
   */
  bool operator!=(const null_iterator &_rhs) const
  {
    return false;
  }

  /**
   * @brief required to support range based for loops
   *
   * @return null_iterator& updated iterator, though the iterator should never
   * actually advance
   */
  null_iterator &operator++()
  {
    return *this;
  }
};

_RAYTRACE_ECS_END