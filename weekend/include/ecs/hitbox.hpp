#pragma once

#include <optional>
#include "prelude.hpp"
_RAYTRACE_ECS_START

struct hit_record
{
  point3 intersection_point;
  vec3 normal;
  f64 intersection_time;

  size_t object_idx;
  // type of hitbox
};

struct sphere
{
  point3 m_center;
  f64 m_radius;
  f64 m_radius_squared;
};

class HitboxAllocator
{
public:
  std::optional<hit_record> hit(const ray &r, interval ray_t) const;
  bool allocate_object(/** need some generic type? or overload */);

public:
  enum class HitboxType
  {
    INVALID = -1,

    sphere,

    COUNT
  };

private:
  // since we ony support spheres rn making it simple
  std::vector<HitboxType> m_types;
  std::vector<size_t> m_subtype_idx;

  struct spheres_allocations
  {
    std::vector<point3> center;
    std::vector<f64> radius;
  } m_spheres;
};

template <class T>
class TextureLoader
{
public:
  void create_material(T mat);
};

class TextureAllocator
{
public:
public:
  enum class TextureType
  {
    INVALID = -1,

    lambertian,
    metal,
    dielectric,

    COUNT
  };

private:
  std::vector<TextureType> m_types;
  std::vector<size_t> m_subtype_idx;

  // one for each types
  TextureLoader<int> m_textures;

  // generally textures are very large so it makes sense to have one "resouce" loaded to share
};

class ECS
{
public:
  bool create_object();

private:
  HitboxAllocator m_hitboxes;
  TextureAllocator m_textures;
};

_RAYTRACE_ECS_END