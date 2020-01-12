typedef struct _Mat4 {
    double x [4][4];
} Mat4;

Mat4 mat4(double, double);
Mat4 add_mat4(Mat4, Mat4);
Mat4 sub_mat4(Mat4, Mat4);
Mat4 mlt_mat4(Mat4, Mat4);
Mat4 dvd_mat4(Mat4, Mat4);
Mat4 dot_mat4(Mat4, Mat4);

Vec4 mlt_vec4_mat4(Vec4, Mat4);