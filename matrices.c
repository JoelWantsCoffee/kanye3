#include "vectors.h"
#include "matrices.h"

Mat4 mat4(double spine, double base) {
    Mat4 out;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) out.x[i][j] = spine;
            else out.x[i][j] = base;
        }
    }
    return out;
}

Mat4 add_mat4(Mat4 a, Mat4 b) {
    Mat4 out;
    for (int i = 0; i < 16; i++) out.x[i % 4][i / 4] = a.x[i % 4][i / 4] + b.x[i % 4][i / 4];
    return out;
}

Mat4 sub_mat4(Mat4 a, Mat4 b) {
    Mat4 out;
    for (int i = 0; i < 16; i++) out.x[i % 4][i / 4] = a.x[i % 4][i / 4] - b.x[i % 4][i / 4];
    return out;
}

Mat4 mlt_mat4(Mat4 a, Mat4 b) {
    Mat4 out;
    for (int i = 0; i < 16; i++) out.x[i % 4][i / 4] = a.x[i % 4][i / 4] * b.x[i % 4][i / 4];
    return out;
}

Mat4 dvd_mat4(Mat4 a, Mat4 b) {
    Mat4 out;
    for (int i = 0; i < 16; i++) out.x[i % 4][i / 4] = a.x[i % 4][i / 4] / b.x[i % 4][i / 4];
    return out;
}

Mat4 dot_mat4(Mat4 a, Mat4 b) {
    Mat4 out;
    for (int i = 0; i < 16; i++) {
        out.x[i % 4][i / 4] = 0;
        for (int j = 0; i < 4; j++) out.x[i % 4][i / 4] += a.x[i % 4][j] * b.x[j][i / 4];
    }
    return out;
}

Vec4 mlt_vec4_mat4(Vec4 v, Mat4 m) {
    Vec4 out = {
        .x = v.x * m.x[0][0] + v.y * m.x[1][0] + v.z * m.x[2][0] + m.x[3][0],
        .y = v.x * m.x[0][1] + v.y * m.x[1][1] + v.z * m.x[2][1] + m.x[3][1],
        .z = v.x * m.x[0][2] + v.y * m.x[1][2] + v.z * m.x[2][2] + m.x[3][2],
        .w = v.x * m.x[0][3] + v.y * m.x[1][3] + v.z * m.x[2][3] + m.x[3][3],
    };
    return out;
}