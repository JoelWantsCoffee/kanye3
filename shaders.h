typedef int (*PixelShader)(void *, void *);

typedef struct _PixelShaderInfo {
    PixelShader shade;
    double ** depthBuffer;
    void * info1;
    void * info2;
    void * info3;
} PixelShaderInfo;

int rgb_to_int(double, double, double);

int fill_white(void *, void *);
int fill_black(void *, void *);
int fill_rand(void *, void *);
int fill_gradient(void *, void *);
int fill_z(void *, void *);
int fill_checkers(void *, void *);

int update_depth_buffer(PixelShaderInfo, Vec4 *);
void run_shader(void *, Vec4 *);

void clear_depth_buffer(void *);
void create_depth_buffer(void *);