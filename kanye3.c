#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 
#include <math.h>

#include "vectors.h"
#include "matrices.h"
#include "transforms.h"

#include <SDL2/SDL.h>

#include "shaders.h"
#include "pipeline.h"


#include <pthread.h>


#define PI 3.141592653589
#define swap(type, a, b) {type tmp; tmp=*a; *a=*b; *b=tmp;}

#define red(c) ((c & 0xFF0000) >> 16)
#define green(c) ((c & 0x00FF00) >> 8)
#define blue(c) ((c & 0x0000FF))

#include <unistd.h>
#include <sys/time.h>


int QUIT = 0;
int keys [1024];
pthread_mutex_t keylock;
pthread_t keycheck;
SDL_Renderer * RENDERER;



Mesh make_cube() {
    Mesh out;
    out.vertices = malloc(sizeof(Vec4) * 8);
    out.verticesLength = 8;
    out.vertices[0] = vec4(-1, -1, -1, 0);
    out.vertices[1] = vec4(1, -1, -1, 0);
    out.vertices[2] = vec4(-1, 1, -1, 0);
    out.vertices[3] = vec4(1, 1, -1, 0);
    out.vertices[4] = vec4(-1, -1, 1, 0);
    out.vertices[5] = vec4(1, -1, 1, 0);
    out.vertices[6] = vec4(-1, 1, 1, 0);
    out.vertices[7] = vec4(1, 1, 1, 0);

    out.indices = malloc(sizeof(int) * 36);
    out.indicesLength = 36;
    int tmp [36] = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4,
    };
    memcpy(out.indices, tmp, sizeof(int) * 36);

    return out;
}

void *checkers(void * arg) {
    arg = arg;
    while (!QUIT) {
        SDL_Event event;
        while( SDL_PollEvent( &event ) ){
            pthread_mutex_lock(&keylock);
            switch( event.type ){
                case SDL_KEYDOWN:
                    keys[event.key.keysym.sym & 0x00FF] = 1;
                    break;

                case SDL_KEYUP:
                    keys[event.key.keysym.sym & 0x00FF] = 0;
                    break;

                case SDL_QUIT:
                    QUIT = 1;
                    break;
            }
            pthread_mutex_unlock(&keylock);
        }
        
    }
    return NULL;
}

// int nada(void * a, void * b) {
//     return 0xFFFFFF;
// }

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * win = SDL_CreateWindow("KANYE3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 768, 768, 0);
    SDL_Renderer * rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_Texture * text = SDL_CreateTexture(rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 768, 768);


    // double pFar = 1;
    // double pNear = 0.2;
    // Mat4 proj = mat4(0, 0);
    // proj.x[0][0] = PI/4;
    // proj.x[1][1] = PI/4;
    // proj.x[2][2] = pFar/(pFar - pNear);
    // proj.x[3][2] = -(pFar * pNear)/(pFar - pNear);
    // proj.x[2][3] = 1;
    
    memset(keys, 0, sizeof(char) * 500);
    pthread_mutex_init(&keylock, NULL);
    pthread_create(&keycheck, NULL, checkers, NULL);

    Vec4 mouseRot = vec4(0,0,0,0);
    Vec4 wasdMove = vec4(0,0,0,0);

    PipeInfo pCube = {
        .screen = vec4(768, 768, 1, 20),
        .meshRotation = vec4(0, 0, 0, 0),
        .meshTranslation = vec4(0, 0, 10, 0),
        .worldRotation = &mouseRot,
        .worldTranslation = &wasdMove,
        .text = text,
        .ps = {
            .shade = fill_checkers,
            .depthBuffer = NULL,
            .info1 = NULL,
            .info2 = NULL,
            .info3 = NULL,
        },
    };

    create_depth_buffer(&pCube);

    PipeInfo pCube2 = pCube;
    pCube2.meshTranslation = vec4(-5, 0, 10, 0);
    pCube2.ps.shade = fill_rand;


    struct timeval t;

    while (1) {
        gettimeofday(&t, NULL);

        clear_depth_buffer(&pCube);
        Mesh cube = make_cube();
        Mesh cube2 = make_cube();

        double tim = (double)(t.tv_sec) + (double)(t.tv_usec) / 1000000;

        tim /= 2;

        pCube.meshRotation = vec4(cos(tim) * PI, sin(sqrt(2.0) * tim) * PI, 0, 0);

        cube.pipeInfo = pCube;
        cube2.pipeInfo = pCube2;


        clear_text(&cube.pipeInfo, fill_gradient);
        pipe_begin(cube);
        pipe_begin(cube2);

        SDL_RenderCopy(rend, text, NULL, NULL);
        SDL_RenderPresent(rend);

        pthread_mutex_lock(&keylock);

        // for (int i = 0; i < 500; i++) {
        //     if (keys[i]) printf("%d: %d\t", i, keys[i]);
        // }

        
        if (keys[119] || keys[115]) {
            float f = cos(mouseRot.y) * 0.1;
            float s = sin(mouseRot.y) * 0.1;
            wasdMove.z -= f * (1 - 2 * keys[115]);
            wasdMove.x += s * (1 - 2 * keys[115]);
        }

        if (keys[100] || keys[97]) {
            float f = cos(mouseRot.y) * 0.1;
            float s = sin(mouseRot.y) * 0.1;
            wasdMove.x -= f * (1 - 2 * keys[97]);
            wasdMove.z -= s * (1 - 2 * keys[97]);
        }

        fflush(stdout);

        int x = 0, y = 0;

        SDL_GetMouseState(&x, &y);

        double hangle = ((x / 768.0) - 0.5) * 2 * PI;
        double vangle = ((y / 768.0) - 0.5) * PI;

        mouseRot.x = vangle;
        mouseRot.y = -hangle;


        if (QUIT) {
            break;
        }

        pthread_mutex_unlock(&keylock);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rend);
    SDL_Quit();
    exit(0);

    return 0;
}