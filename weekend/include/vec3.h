#ifndef WEEKEND_VEC3_H
#define WEEKEND_VEC3_H

#include <iostream>
#include <cmath>
#include "types.h"
#include "utils.h"
/**
 * TODO: return to this for a DOD approach
 * TODO: offer a f32 version for memory footprint
 * TODO: simd usage??
 */

class vec3 final
{
public:
    f64 m_repr[3];

    vec3() : m_repr{0, 0, 0} {}
    vec3(const f64 x, const f64 y, const f64 z) : m_repr{x, y, z} {}

    // All these methods should be relatively cheap and ok to inline

    inline f64 x() const { return m_repr[0]; }
    inline f64 y() const { return m_repr[1]; }
    inline f64 z() const { return m_repr[2]; }

    inline vec3 operator-() const { return vec3(-x(), -y(), -z()); }
    /** Careful unchecked */
    f64 operator[](const size_t i) const { return m_repr[i]; }
    f64 &operator[](const size_t i) { return m_repr[i]; }

    inline vec3 &operator+=(const vec3 &other)
    {
        m_repr[0] += other.x();
        m_repr[1] += other.y();
        m_repr[2] += other.z();
        return *this;
    }

    inline vec3 &operator*=(const f64 &scalar)
    {
        m_repr[0] *= scalar;
        m_repr[1] *= scalar;
        m_repr[2] *= scalar;
        return *this;
    }
    friend inline vec3 operator*(const vec3 &self, const f64 &scalar)
    {
        return vec3(
            self.m_repr[0] * scalar,
            self.m_repr[1] * scalar,
            self.m_repr[2] * scalar);
    }
    friend inline vec3 operator*(const f64 &scalar, const vec3 &self)
    {
        // communtative (theoretically though IEEE might have some issues?)
        return self * scalar;
    }

    inline vec3 &operator/=(const f64 &scalar)
    {
        *this *= (1 / scalar);
        return *this;
    }
    friend inline vec3 operator/(const vec3 &self, const f64 &scalar)
    {
        return self * (1 / scalar);
    }

    inline f64 len() const
    {
        return std::sqrt(len_squared());
    }

    inline f64 len_squared() const
    {
        return m_repr[0] * m_repr[0] + m_repr[1] * m_repr[1] + m_repr[2] * m_repr[2];
    }

    // maybe useful idk
    static inline vec3 zero() { return vec3(); }
    static inline vec3 one() { return vec3(1, 1, 1); }

    inline vec3 unit() const
    {
        return (*this) / (this->len());
    }

    static vec3 random()
    {
        return vec3(random_f64(), random_f64(), random_f64());
    }
    static vec3 random(f64 min, f64 max)
    {
        return vec3(random_f64(min, max), random_f64(min, max), random_f64(min, max));
    }

    bool near_zero() const
    {
        static const f64 EPSILON = 1e-8;
        return (std::fabs(x() < EPSILON)) && (std::fabs(y() < EPSILON)) && (std::fabs(z() < EPSILON));
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;

/**
 * Other vec3 utils
 * TODO: see if making these static member functions makes more sense
 */

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(
        u.x() + v.x(),
        u.y() + v.y(),
        u.z() + v.z());
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(
        u.x() - v.x(),
        u.y() - v.y(),
        u.z() - v.z());
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(
        u.x() * v.x(),
        u.y() * v.y(),
        u.z() * v.z());
}

inline vec3 operator/(const vec3 &u, const vec3 &v)
{
    return vec3(
        u.x() / v.x(),
        u.y() / v.y(),
        u.z() / v.z());
}

inline bool operator==(const vec3 &u, const vec3 &v)
{
    return u.x() == v.x() && u.y() == v.y() && u.z() == v.z();
}

inline f64 dot(const vec3 &u, const vec3 &v)
{
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(
        u.y() * v.z() - u.z() * v.y(),
        u.z() * v.x() - u.x() * v.z(),
        u.x() * v.y() - u.y() * v.x());
}

/**
 * Dumping helper function for printing here to optionally include in case i dont wanna bloat a given binary
 */

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.len_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector()
{
    return random_in_unit_sphere().unit();
}

inline vec3 random_on_hemisphere(const vec3 &normal)
{
    /**
     * TODO: This seems bad, probably better to see if we can take a normal
     * rotate by some random angle theta then another angle phi
     */
    vec3 on_unit_sphere = random_unit_vector();
    // In the same hemisphere as the normal
    if (dot(on_unit_sphere, normal) > 0.0)
    {
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, f64 etaI_over_etaT)
{
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perpendicular = etaI_over_etaT * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perpendicular.len_squared())) * n;
    return r_out_parallel + r_out_perpendicular;
}

#endif