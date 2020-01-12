#include <SDL2/SDL.h>

#include "vectors.h"
#include "shaders.h"
#include "pipeline.h"

#define MIN(a, b) (a < b) ? (a) : (b)
#define red(c) ((c & 0xFF0000) >> 16)
#define green(c) ((c & 0x00FF00) >> 8)
#define blue(c) ((c & 0x0000FF))

int rgb_to_int(double r, double g, double b) {
    if (r > 1) r = 1; 
    if (g > 1) g = 1; 
    if (b > 1) b = 1;
    if (r < 0) r = 0; 
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    int red = ((int) (0xFF * r)) << 16;
    int green = ((int) (0xFF * g)) << 8;
    int blue = 0xFF * b;
    return red + green + blue;
}

int fill_white(void * a, void * b) {
    a = a; b = b;
    return 0xFFFFFF;
}

int fill_black(void * a, void * b) {
    a = a; b = b;
    return 0x000000;
}


int fill_rand(void * a, void * b) {
    a = a; b = b;
    return rand() % 0xFFFFFF;
}

int fill_gradient(void * a, void * b) {
    a = a;
    Vec4 * v = b;
    int red = (int) (0xFF * (v->x / 768.0)) << 16;
    int green = (int) (0xFF * (1.0 - (v->x / 768.0))) << 8;
    int blue = 0xFF;
    return red + green + blue;
}

int fill_z(void * a, void * b) {
    a = a;
    Vec4 * v = b;
    return rgb_to_int(v->z, v->z, v->z);
}

int fill_checkers(void * a, void * b) {
    a = a;
    Vec4 * v = b;
    if ((int) v->x % 2 != (int) v->y % 2) {
        return fill_z(a, b);
    } else {
        return fill_gradient(a, b);
    }
}

int update_depth_buffer(PixelShaderInfo ps, Vec4 * p) {
    if (ps.depthBuffer[(int) p->x][(int) p->y] > p->z) {
        ps.depthBuffer[(int) p->x][(int) p->y] = p->z;
        return 1;
    }
    return 0;
}

void clear_depth_buffer(void * pipeinf) {
    PipeInfo * inf = pipeinf;
    for (int i = 0; i < (int) inf->screen.x; i++) {
        for (int j = 0; j < (int) inf->screen.y; j++) {
            inf->ps.depthBuffer[i][j] = inf->screen.z;
        }
    }
}

void create_depth_buffer(void * pipeinf) {
    PipeInfo * inf = pipeinf;
    inf->ps.depthBuffer = malloc(sizeof(double*) * ((int) inf->screen.x));
    for (int i = 0; i < (int) inf->screen.x; i++)
    inf->ps.depthBuffer[i] = malloc(sizeof(double*) * ((int) inf->screen.y));
    clear_depth_buffer(pipeinf);
}

void run_shader(void * pipeinf, Vec4 * p) {
    PipeInfo * inf = pipeinf;
    int col = inf->ps.shade(inf, p);
    if (p->w && update_depth_buffer(inf->ps, p)) {
        Uint32 pixelPosition = p->y * (inf->pitch / sizeof(unsigned int)) + p->x;
        ((Uint32 *) inf->pixels)[pixelPosition] = col;
    }
}