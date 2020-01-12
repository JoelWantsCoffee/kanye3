#include <math.h>

#include "vectors.h"


Vec4 vec4(double x, double y, double z, double w) {
    Vec4 out = {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
    return out;
}

Vec4 add_vec4(Vec4 a, Vec4 b) {
    return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vec4 sub_vec4(Vec4 a, Vec4 b) {
    return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

double dot_vec4(Vec4 a, Vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec4 crs_vec4(Vec4 a, Vec4 b) {
    return vec4(a.y * b.z - a.z * b.y, a.y * b.x - a.x * b.y, a.x * b.y - a.y * b.x, a.w * b.w);
}

Vec4 scl_vec4(Vec4 v, double s) {
    return vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vec4 itp_vec4(Vec4 a, Vec4 b, double delta) {
    return vec4(a.x + delta * (b.x - a.x), a.y + delta * (b.y - a.y), a.z + delta * (b.z - a.z), a.w + delta * (b.w - a.w));
}

Vec4 nm3_vec4(Vec4 v) {
    double l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return vec4(v.x / l, v.y / l, v.z / l, v.w);
}

double mag_vec4(Vec4 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double dst_vec4(Vec4 a, Vec4 b) {
    double x = a.x - b.x;
    double y = a.y - b.y;
    double z = a.z - b.z;
    double w = a.w - b.w;
    return sqrt(x * x + y * y + z * z + w * w);
}