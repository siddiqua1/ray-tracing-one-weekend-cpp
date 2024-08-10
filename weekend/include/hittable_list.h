#ifndef WEEKEND_HITTABLE_LIST_H
#define WEEKEND_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list final : public hittable
{
public:
  std::vector<shared_ptr<hittable>> m_objects;

  hittable_list() = default;
  hittable_list(shared_ptr<hittable> object) { add(object); }

  void clear() { m_objects.clear(); }
  void add(shared_ptr<hittable> object)
  {
    m_objects.push_back(object);
  }

  std::optional<hit_record> hit(const ray &r, f64 t_min, f64 t_max) const override
  {
    std::optional<hit_record> closest_valid_hit;
    f64 closest_hit = t_max;

    for (const auto &object : m_objects)
    {
      auto maybe_hit = object->hit(r, t_min, closest_hit);
      if (maybe_hit.has_value())
      {
        const auto hit = maybe_hit.value();
        closest_hit = hit.t;
        closest_valid_hit = hit;
      }
    }
    return closest_valid_hit;
  }
};

#endif