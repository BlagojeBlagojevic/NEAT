#define NUMBER_OF_SPICES 1
#define LR 0.005
#define MUTATION_RATE 0.1
#define NUMBER_OF_LAYER 3
#define NUMBER_OF_NEURON 20
#define NINPUTS 764
#define NOUT 10
#define NEAT_IMPLEMETATION
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_SWISH
#define ENABLE_MUTATION 0.2
#define DISABLE_MUTATION 0.04
//#define NEAT_IMPLEMETATION
#define ENABLE_SOFTMAX
#define T 10
#include "neat.h"
#include<string.h>
#define NUM_OF_IMAGES 10*30

#include<float.h>
#define IMAGE_IMPLEMENTATION
#include "image.h"


#include<SDL2/SDL.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
#include<SDL2/SDL_image.h>
#include<assert.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

#undef main
#define width  280
#define height 280




void Main_Renderer(SDL_Renderer *renderer, SDL_Rect *rect, Mat point){

     SDL_SetRenderDrawColor(renderer,255,255,255,0);
    for (size_t y = 0; y < 28; y++)
    {
        for (size_t x = 0; x < 28; x++)
        {
            if(MATRIX_SHIFT(point,y,x))
                SDL_RenderFillRect(renderer, &rect[x + y*28]);
        }
        
    }
     SDL_SetRenderDrawColor(renderer,0,0,0,0);
     SDL_RenderPresent(renderer);
    

}
void Procss_Mouse_Input(Mat point) {

	int X = 0, Y = 0;
	
	SDL_GetMouseState(&X, &Y);
	
    X = X / 10;
	Y = Y / 10;
    MATRIX_SHIFT(point, Y, X) = 1;
	


}






int main(){

    SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(width,height,SDL_RENDERER_ACCELERATED | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,&window,&renderer);
    SDL_Rect rect[28*28];
    Mat point = matrix_alloc(28,28);
    NEAT population[NUMBER_OF_SPICES + 1];
    neat_alloc(population);
    neat_load(population,"a.bin");    
    for (size_t y = 0; y < 28; y++)
    {
        for (size_t x = 0; x < 28; x++)
        {
            rect[x + y*28].h = 10;
            rect[x + y*28].w = 10;
            rect[x + y*28].x = x*10;
            rect[x + y*28].y = y*10;
            MATRIX_SHIFT(point, y, x) = 0;
        }
        
        
    }
    
    while (1)
    {
        static SDL_Event event;
        while(SDL_WaitEvent(&event)){
            if(event.type == SDL_MOUSEBUTTONDOWN) {
				Procss_Mouse_Input(point);
				}

            else if(event.type == SDL_QUIT){
                return 0;
            }

            
            
            //matrix_print(point,"a");
            SDL_RenderClear(renderer);
            Main_Renderer(renderer,rect,point);
           
            for (size_t i = 0; i < NINPUTS; i++)
            {
                population[0].input.elem[i] = (float)point.elem[i];
            }
            neat_forward(population,1);
            MATRIX_PRINT(population[0].out_softmax);

        }
        
    }
    

    return 0;
}