typedef struct _PipeInfo {
    Vec4 screen;
    Vec4 meshRotation;
    Vec4 meshTranslation;
    Vec4 * worldRotation;
    Vec4 * worldTranslation;
    void * text;
    void * pixels;
    int pitch;
    PixelShaderInfo ps;
} PipeInfo;

typedef struct _Mesh {
    PipeInfo pipeInfo;
    Vec4 * vertices;
    int verticesLength;
    int * indices;
    int indicesLength;
} Mesh;

void pipe_begin(Mesh m);
Vec4 * pipe_vertex_transform(PipeInfo * p, Vec4 * vertices, int verticesLength);
void pipe_triangle_assembler(PipeInfo * p, Vec4 * vertices, int verticesLength, int * indices, int indicesLength);
void pipe_process_triangle(PipeInfo * p, Vec4 a, Vec4 b, Vec4 c);
void clear_text(PipeInfo * inf, int (*shade)(void *, void *));