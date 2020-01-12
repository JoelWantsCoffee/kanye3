typedef struct _Vec4 {
    double x;
    double y;
    double z;
    double w;
} Vec4;

Vec4 vec4(double, double, double, double);
Vec4 add_vec4(Vec4, Vec4);
Vec4 sub_vec4(Vec4, Vec4);
Vec4 crs_vec4(Vec4, Vec4);
Vec4 nm3_vec4(Vec4);
Vec4 scl_vec4(Vec4, double); 
Vec4 itp_vec4(Vec4, Vec4, double);
double mag_vec4(Vec4);
double dot_vec4(Vec4, Vec4);
double dst_vec4(Vec4, Vec4);