#pragma once

#include "prelude.hpp"

_RAYTRACE_ECS_START

template <class ImplAllocator, class Iterator>
class I_Allocator
{
public: /** main interface methods */
public: /** iteration support */
  Iterator begin()
  {
    return static_cast<ImplAllocator *>(this)->begin_impl();
  }
  Iterator end()
  {
    return static_cast<ImplAllocator *>(this)->end_impl();
  }
};

_RAYTRACE_ECS_END