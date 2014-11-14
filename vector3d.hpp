#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <cmath>

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

    vector3d& rotate_x(double roll) {
        auto r = std::sqrt(y*y + z*z);
        if (r != 0) {
            auto old = std::atan2(y, z);
            y = std::sin(old-roll) * r;
            z = std::cos(old-roll) * r;
        }
        return *this;
    }

    vector3d& rotate_y(double pitch) {
        auto r = std::sqrt(z*z + x*x);
        if (r != 0) {
            auto old = std::atan2(z, x);
            z = std::sin(old-pitch) * r;
            z = std::cos(old-pitch) * r;
        }
        return *this;
    }

    vector3d& rotate_z(double yaw) {
        auto r = std::sqrt(y*y + x*x);
        if (r != 0) {
            auto old = std::atan2(y, x);
            y = std::sin(old-yaw) * r;
            x = std::cos(old-yaw) * r;
        }
        return *this;
    }
};

vector3d operator + (vector3d lhs, const vector3d& rhs) {
    return lhs += rhs;
}

vector3d operator - (vector3d lhs, const vector3d& rhs) {
    return lhs -= rhs;
}

vector3d operator * (vector3d lhs, double rhs) {
    return lhs *= rhs;
}

vector3d operator / (vector3d lhs, double rhs) {
    return lhs /= rhs;
}

vector3d operator * (double lhs, vector3d rhs) {
    return rhs *= lhs;
}

double operator * (vector3d lhs, const vector3d& rhs) {
    return lhs.dot(rhs);
}

#endif
