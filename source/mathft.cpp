#include "mathft.hpp"

Mat4 multiply(const Mat4 &a, const Mat4 &b) {
    Mat4 m{};

    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            float sum = 0.0f;

            for (int k = 0; k < 4; ++k)
                sum += a[k][r] * b[c][k];
            m[c][r] = sum;
        }
    }

    return m;
}

Mat4 rotate(const Mat4 &mat, float angle, const Vec3 &axis) {
    Vec3 a = normalize(axis);
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;

    Mat4 m = identity();

    m[0][0] = c + a.x * a.x * t;
    m[0][1] = a.y * a.x * t + a.z * s;
    m[0][2] = a.z * a.x * t - a.y * s;

    m[1][0] = a.x * a.y * t - a.z * s;
    m[1][1] = c + a.y * a.y * t;
    m[1][2] = a.z * a.y * t + a.x * s;

    m[2][0] = a.x * a.z * t + a.y * s;
    m[2][1] = a.y * a.z * t - a.x * s;
    m[2][2] = c + a.z * a.z * t;

    return multiply(mat, m);
}

Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
    Vec3 f = normalize(center - eye);
    Vec3 s = normalize(cross(f, up));
    Vec3 u = cross(s, f);

    Mat4 m = identity();

    m[0][0] = s.x;
    m[0][1] = u.x; 
    m[0][2] = -f.x;
    m[0][3] = 0.0f;

    m[1][1] = u.y; 
    m[1][0] = s.y;
    m[1][2] = -f.y;
    m[1][3] = 0.0f;

    m[2][0] = s.z;
    m[2][1] = u.z; 
    m[2][2] = -f.z;
    m[2][3] = 0.0f;

    m[3][0] = -dot(s, eye);
    m[3][1] = -dot(u, eye);
    m[3][2] =  dot(f, eye);
    m[3][3] = 1.0f;

    return m;
}

Mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
    float f = 1.0f / tan(fovy * 0.5f);

    Mat4 m{};

    m[0][0] = f / aspect;
    m[1][1] = f;

    m[2][2] = zFar / (zNear - zFar);
    m[2][3] = -1.0f;

    m[3][2] = (zFar * zNear) / (zNear - zFar);

    return m;
}
