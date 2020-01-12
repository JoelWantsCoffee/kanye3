#include "vectors.h"
#include "matrices.h"
#include "transforms.h"
#include "shaders.h"
#include "pipeline.h"

#include <SDL2/SDL.h>

#define MIN(a, b) (a < b) ? (a):(b)
#define MAX(a, b) (a > b) ? (a):(b)
#define swap(type, a, b) {type tmp; tmp=*a; *a=*b; *b=tmp;}
#define red(c) ((c & 0xFF0000) >> 16)
#define green(c) ((c & 0x00FF00) >> 8)
#define blue(c) ((c & 0x0000FF))

void draw_ftop_triangle(PipeInfo * inf, Vec4 a, Vec4 b, Vec4 c) {
    double dydx0 = (c.x - a.x) / (c.y - a.y);
    double dydx1 = (c.x - b.x) / (c.y - b.y);

    double dydz0 = (c.z - a.z) / (c.y - a.y);
    double dydz1 = (c.z - b.z) / (c.y - b.y);

    int yStart = MIN(ceil(a.y - 0.5), inf->screen.y); 
    yStart = MAX(yStart, 0);
    int yEnd = MIN(ceil(c.y - 0.5), inf->screen.y); 
    yEnd = MAX(yEnd, 0);

    for (int y = yStart; y < yEnd; y++) {
        int x0 = MIN(ceil(dydx0 * ((double) y + 0.5 - a.y) + a.x - 0.5), inf->screen.x);
        x0 = MAX(x0, 0);
        int x1 = MIN(ceil(dydx1 * ((double) y + 0.5 - b.y) + b.x - 0.5), inf->screen.x);
        x1 = MAX(x1, 0);

        double z0 = (dydz0 * ((double) y + 0.5 - a.y) + a.z - 0.5);
        double z1 = (dydz1 * ((double) y + 0.5 - b.y) + b.z - 0.5);

        double xFrac = 0, xFracStep = 1.0/(x1 - x0);

        for (int x = x0; x < x1; x++, xFrac += xFracStep) {
            double z = z0 + xFrac*(z1 - z0);
            Vec4 pos = vec4(x, y, z, 1);
            run_shader(inf, &pos);
        }
    }
}

void clear_text(PipeInfo * inf, int (*shade)(void *, void *)) {
    SDL_LockTexture(inf->text, NULL, (void**)&inf->pixels, &inf->pitch);

    for (int i = 0; i < inf->screen.x; i++) {
        for (int j = 0; j < inf->screen.y; j++) {
            Vec4 p = vec4(i, j, 0, 0);
            int col = 0;
            if (shade) {
                col = shade(inf, &p);
            } else {
                col = inf->ps.shade(inf, &p);
            }
             
            ((Uint32 *) inf->pixels)[j * (inf->pitch / sizeof(unsigned int)) + i] = col;
        }
    }

    SDL_UnlockTexture(inf->text);
}

void draw_fbot_triangle(PipeInfo * inf, Vec4 a, Vec4 b, Vec4 c) {
    double dydx0 = (b.x - a.x) / (b.y - a.y);
    double dydx1 = (c.x - a.x) / (c.y - a.y);

    double dydz0 = (b.z - a.z) / (b.y - a.y);
    double dydz1 = (c.z - a.z) / (c.y - a.y);

    int yStart = MIN(ceil(a.y - 0.5), inf->screen.y);
    yStart = MAX(yStart, 0);
    int yEnd = MIN(ceil(c.y - 0.5), inf->screen.y);
    yEnd = MAX(yEnd, 0);

    for (int y = yStart; y < yEnd; y++) {
        int x0 = MIN(ceil(dydx0 * (((double) y) + 0.5 - a.y) + a.x - 0.5), inf->screen.x);
        x0 = MAX(x0, 0);
        int x1 = MIN(ceil(dydx1 * (((double) y) + 0.5 - a.y) + a.x - 0.5), inf->screen.x);
        x1 = MAX(x1, 0);

        double z0 = (dydz0 * (((double) y) + 0.5 - a.y) + a.z - 0.5);
        double z1 = (dydz1 * (((double) y) + 0.5 - a.y) + a.z - 0.5);

        double xFrac = 0, xFracStep = 1.0/(x1 - x0);

        for (int x = x0; x < x1; x++, xFrac += xFracStep) {
            double z = z0 + xFrac*(z1 - z0);
            Vec4 pos = vec4(x, y, z, 1);
            run_shader(inf, &pos);
        }
    }
    
}

void draw_triangle(PipeInfo * inf, Vec4 a, Vec4 b, Vec4 c) {
    Vec4 * v0 = &a;
    Vec4 * v1 = &b;
    Vec4 * v2 = &c;

    if (v0->y > v1->y) swap(Vec4, v0, v1);
    if (v1->y > v2->y) swap(Vec4, v1, v2);
    if (v0->y > v1->y) swap(Vec4, v0, v1);

    // if (v0->y == v2->y) return;

    SDL_LockTexture(inf->text, NULL, (void**)&inf->pixels, &inf->pitch);

    if (v0->y == v1->y) {
        if (v0->x > v1->x) swap(Vec4, v0, v1);
        draw_ftop_triangle(inf, *v0, *v1, *v2);
    } else if (v1->y == v2->y) {
        if (v1->x > v2->x) swap(Vec4, v1, v2);
        draw_fbot_triangle(inf, *v0, *v1, *v2);
    } else {
        double s = (v1->y - v0->y) / (v2->y - v0->y);
        Vec4 v3 = add_vec4(*v0, scl_vec4(sub_vec4(*v2, *v0), s));
        if (v1->x < v3.x) {
            draw_fbot_triangle(inf, *v0, *v1, v3);
            draw_ftop_triangle(inf, *v1, v3, *v2);
        } else {
            draw_fbot_triangle(inf, *v0, v3, *v1);
            draw_ftop_triangle(inf, v3, *v1, *v2);
        }
    }

    SDL_UnlockTexture(inf->text);
}

void pipe_begin(Mesh m) {
    Vec4 * transVertices = pipe_vertex_transform(&m.pipeInfo, m.vertices, m.verticesLength);
    pipe_triangle_assembler(&m.pipeInfo, transVertices, m.verticesLength, m.indices, m.indicesLength);
    free(transVertices);
}

Vec4 * pipe_vertex_transform(PipeInfo * p, Vec4 * vertices, int verticesLength) {
    Vec4 * out = malloc(sizeof(Vec4) * verticesLength);
    for (int i = 0; i < verticesLength; i++) {
        out[i] = t_rotate_vec4(vertices[i], p->meshRotation.x, p->meshRotation.y, p->meshRotation.z);
        out[i] = add_vec4(out[i], p->meshTranslation);
        if (p->worldTranslation) {
            out[i] = add_vec4(out[i], *p->worldTranslation);
        }
        if (p->worldRotation) {
            out[i] = t_rotate_vec4(out[i], p->worldRotation->x, p->worldRotation->y, p->worldRotation->z);
        }
    }
    return out;
}

void pipe_triangle_assembler(PipeInfo * p, Vec4 * vertices, int verticesLength, int * indices, int indicesLength) {
    Mat4 proj = create_projection_matrix(p->screen);
    Vec4 scr = vec4(0, 0, 0, 1);
    scr = mlt_vec4_mat4(scr, proj);
    scr.w = 0;
    for (int i = 0; i < indicesLength / 3; i++) {
        if ((indices[i * 3] > verticesLength) || (indices[i * 3 + 1] > verticesLength) || (indices[i * 3 + 2] > verticesLength)) {
            fprintf(stderr, "Index/Vertiex count missmatch\n");
            fflush(stderr);
            exit(2);
        }
        Vec4 a = vertices[indices[i * 3    ]];
        Vec4 b = vertices[indices[i * 3 + 1]];
        Vec4 c = vertices[indices[i * 3 + 2]];
        // if( (b - a) % (c - a) * Vec3(v0.pos - scr) <= 0.0f )
        // if ((dot_vec4(crs_vec4(sub_vec4(b, a), sub_vec4(c, a)), sub_vec4(a, scr)) <= 0.0)) {
            pipe_process_triangle(p, a, b, c);
        // }
    }
    
}

void pipe_post_triangle(PipeInfo * p, Vec4 a, Vec4 b, Vec4 c) {
    a = t_world_screen_vec4(a, p->screen);
    b = t_world_screen_vec4(b, p->screen);
    c = t_world_screen_vec4(c, p->screen);

    draw_triangle(p, a, b, c);
}

void clip1(PipeInfo * p, Vec4 a, Vec4 b, Vec4 c) {
    double alphaB = -a.z / (b.z - a.z);
    double alphaC = -a.z / (c.z - a.z);

    Vec4 na = itp_vec4(a, b, alphaB);
    Vec4 nb = itp_vec4(a, c, alphaC);

    pipe_post_triangle(p, na, b, c);
    pipe_post_triangle(p, na, nb, c);
}

void clip2(PipeInfo * p, Vec4 a, Vec4 b, Vec4 c) {
    double alphaA = -a.z / (c.z - a.z);
    double alphaB = -b.z / (c.z - b.z);

    a = itp_vec4(a, c, alphaA);
    b = itp_vec4(b, c, alphaB);

    pipe_post_triangle(p, a, b, c);
}

void clip_triangle(PipeInfo * p, Vec4 a, Vec4 b, Vec4 c) {
    //delet bois

    // fprintf(stderr, "%lF, %lF\n", a.x, a.w);
    if (a.x > a.w && b.x > b.w && c.x > c.w) return;
    if (a.y > a.w && b.y > b.w && c.y > c.w) return;
    if (a.z > a.w && b.z > b.w && c.z > c.w) return;
    if (a.x < -a.w && b.x < -b.w && c.x < -c.w) return;
    if (a.y < -a.w && b.y < -b.w && c.y < -c.w) return;
    if (a.z < 0.0 && b.z < 0.0 && c.z < 0.0) return;

    if (a.z < 0.0) {
        if (b.z < 0.0) {
            clip2(p, a, b, c);
        } else if (c.z < 0.0) {
            clip2(p, a, c, b);
        } else {
            clip1(p, a, b, c);
        }
    } else if (b.z < 0.0) {
        if (c.z < 0.0) {
            clip2(p, b, c, a);
        } else {
            clip1(p, b, a, c);
        }
    } else if (c.z < 0.0) {
        clip1(p, c, b, a);
    } else {
        pipe_post_triangle(p, a, b, c);
    }
}

void pipe_process_triangle(PipeInfo * p, Vec4 a, Vec4 b, Vec4 c) {
    Mat4 proj = create_projection_matrix(p->screen);
    a = mlt_vec4_mat4(a, proj); 
    b = mlt_vec4_mat4(b, proj);
    c = mlt_vec4_mat4(c, proj);
    //CLIP
    clip_triangle(p, a, b, c);
}