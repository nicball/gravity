#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "math.h"

struct vector3d {
    double x, y, z;

    vector3d& operator += (const vector3d& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    vector3d& operator -= (const vector3d& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    vector3d& operator *= (double rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    vector3d& operator /= (double rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    double dot(const vector3d& rhs) const {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }

    double length() const {
        return std::sqrt(dot(*this));
    }

    vector3d& set_length(double d) {
        auto o = length();
        (*this *= d) /= o;
        return *this;
    }

    vector3d& clear() {
        x = y = z = 0;
        return *this;
    }

    vector3d& normalize() {
        *this /= length();
        return *this;
    }

    vector3d& rotate_x(double rad) {
        auto r = std::sqrt(y*y + z*z);
        if (r != 0) {
            auto old = std::atan2(z, y);
            z = std::sin(old + rad) * r;
            y = std::cos(old + rad) * r;
        }
        return *this;
    }

    vector3d& rotate_y(double rad) {
        auto r = std::sqrt(x*x + z*z);
        if (r != 0) {
            auto old = std::atan2(z, x);
            z = std::sin(old + rad) * r;
            x = std::cos(old + rad) * r;
        }
        return *this;
    }

    vector3d& rotate_z(double rad) {
        auto r = std::sqrt(x*x + y*y);
        if (r != 0) {
            auto old = std::atan2(y, x);
            y = std::sin(old + rad) * r;
            x = std::cos(old + rad) * r;
        }
        return *this;
    }
};

inline vector3d operator + (vector3d lhs, const vector3d& rhs) {
    return lhs += rhs;
}

inline vector3d operator - (vector3d lhs, const vector3d& rhs) {
    return lhs -= rhs;
}

inline vector3d operator * (vector3d lhs, double rhs) {
    return lhs *= rhs;
}

inline vector3d operator / (vector3d lhs, double rhs) {
    return lhs /= rhs;
}

inline vector3d operator * (double lhs, vector3d rhs) {
    return rhs *= lhs;
}

inline double operator * (vector3d lhs, const vector3d& rhs) {
    return lhs.dot(rhs);
}

#endif
