#pragma once

#include <cmath>
#include <array>

using namespace std;

struct Vec2 {
    float x{0};
    float y{0};
};

struct Vec3 {
    float x{0};
    float y{0};
    float z{0};

    Vec3() = default;                                           // Défaut
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {} // 3 floats
    explicit Vec3(float v) : x(v), y(v), z(v) {}                // 1 float
};

inline Vec3 operator+(const Vec3 &a, const Vec3 &b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vec3 operator-(const Vec3 &a, const Vec3 &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vec3 operator*(const Vec3 &v, float s) {
    return {v.x * s, v.y * s, v.z * s};
}

inline float dot(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

inline Vec3 normalize(const Vec3 &v) {
    float len = sqrt(dot(v, v));

    if (len == 0) return {0,0,0};

    return v * (1.0f / len);
}

struct Mat4 {
    array<float, 16>    data{};

    float* operator[](size_t column) { return &data[4 * column]; }
    const float* operator[](size_t column) const { return &data[4 * column]; }
};

inline Mat4 identity() {
    Mat4 m{};

    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;

    return m;
}

Mat4 multiply(const Mat4 &a, const Mat4 &b);
Mat4 rotate(const Mat4 &mat, float angle, const Vec3 &axis);
Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
Mat4 perspective(float fovy, float aspect, float zNear, float zFar);
inline float radians(float degrees) { return degrees * static_cast<float>(M_PI) / 180.0f; }
