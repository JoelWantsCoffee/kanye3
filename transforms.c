#include "math.h"
#include "vectors.h"
#include "matrices.h"
#include "transforms.h"

Mat4 create_projection_matrix(Vec4 screen) {
    Mat4 out = mat4(0, 0);
    double w = 2.0;
    double h = 2.0;
    double n = screen.z;
    double f = screen.w;
    out.x[0][0] = 2.0 * n / w;
    out.x[1][1] = 2.0 * n / h;
    out.x[2][2] = f / (f - n);
    out.x[3][2] = -n * f / (f - n);
    out.x[2][3] = 1.0;
    return out;
}

Vec4 t_screen_world_vec4(Vec4 in, int screenSpaceWidth, int screenSpaceHeight) {
    return vec4((2.0 * in.x / screenSpaceWidth) - 1.0, -(2.0 * in.y / screenSpaceHeight) + 1.0, in.z, in.w);
}

Vec4 t_world_screen_vec4(Vec4 in, Vec4 screen) {
    double winv = 1.0 / in.w;
    in = scl_vec4(in, winv);
    in.w = winv;
    return vec4((in.x + 1)*0.5*screen.x, (in.y + 1)*0.5*screen.y, in.z, in.w);
}

Vec4 t_rotate_vec4(Vec4 v, double xrot, double yrot, double zrot) {
    Mat4 m = mat4(1, 0);
    m.x[0][0] = cos(yrot);
    m.x[2][2] = cos(yrot);
    m.x[2][0] = sin(yrot);
    m.x[0][2] = -sin(yrot);
    v = mlt_vec4_mat4(v, m);
    m = mat4(1, 0);
    m.x[1][1] = cos(xrot);
    m.x[2][2] = cos(xrot);
    m.x[2][1] = -sin(xrot);
    m.x[1][2] = sin(xrot);
    v = mlt_vec4_mat4(v, m);
    m = mat4(1, 0);
    m.x[0][0] = cos(zrot);
    m.x[1][0] = -sin(zrot);
    m.x[1][1] = cos(zrot);
    m.x[0][1] = sin(zrot);
    v = mlt_vec4_mat4(v, m);
    return v;
}