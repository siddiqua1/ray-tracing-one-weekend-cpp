#pragma once

#include "vec3.hpp"

namespace weekend::core
{

    class ray
    {
    public:
        ray() {}
        ray(const point3 &origin, const vec3 &direction) : m_origin(origin), m_direction(direction)
        {
        }

        const point3 &origin() const { return m_origin; }
        const vec3 &direction() const { return m_direction; }

        point3 at(f64 t) const
        {
            return m_origin + t * m_direction;
        }

    private:
        point3 m_origin{};
        vec3 m_direction{};
    };
}