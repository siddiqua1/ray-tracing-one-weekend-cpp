#ifndef WEEKEND_VEC3_H
#define WEEKEND_VEC3_H

#include <cmath>
#include "types.h"
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
    static friend inline vec3 operator*(const vec3 &self, const f64 &scalar)
    {
        return vec3(
            self.m_repr[0] * scalar,
            self.m_repr[1] * scalar,
            self.m_repr[2] * scalar);
    }
    static friend inline vec3 operator*(const f64 &scalar, const vec3 &self)
    {
        // communtative (theoretically though IEEE might have some issues?)
        return self * scalar;
    }

    inline vec3 &operator/=(const f64 &scalar)
    {
        *this *= (1 / scalar);
        return *this;
    }
    static friend inline vec3 operator/(const vec3 &self, const f64 &scalar)
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

#endif