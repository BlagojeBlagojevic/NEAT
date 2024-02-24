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
//#include<SDL2/SDL_image.h>
//#include<SDL2/SDl_timer.h>
#include<assert.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
//#include<stdboll.h>
#undef main
#define width  500
#define height 500



#define NUMBER_OF_SPICES 20
#define NUMBER_OF_SPICES_IN_CROSOWER 1
#define LR 0.2
#define MUTATION_RATE 0.6
#define NUMBER_OF_LAYER 3
#define NUMBER_OF_NEURON 10
#define NINPUTS 7
#define NOUT 2
#define NEAT_IMPLEMETATION
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_TANH
#include "neat.h"
size_t curently_alive  = NUMBER_OF_SPICES;   //Must be equal to NUMBER_OF_SPICES
NEAT population[NUMBER_OF_SPICES];
typedef struct {

	SDL_Rect  left;
	SDL_Rect  right;
	SDL_Rect  ball;
	SDL_Point ball_speed;
	//void
	SDL_Color color;
	int is_allive;

	} PONG;

Pong_Init(PONG *Pong) {
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		Pong[i].is_allive = 1;
		Pong[i].left.w  = 10;
		Pong[i].right.w = 10;
		Pong[i].left.h  = 70;
		Pong[i].right.h = 70;
		Pong[i].left.x  = 0;
		Pong[i].right.x = width  - 10 ;
		Pong[i].left.y  = height / 2;
		Pong[i].right.y = height / 2;
		Pong[i].color.r = rand() % 255;
		Pong[i].color.g = rand() % 255;
		Pong[i].color.b = rand() % 255;
		Pong[i].ball.w  = 15;
		Pong[i].ball.h  = 15;
		Pong[i].ball.x  = width   / 2;
		Pong[i].ball.y  = height  / 2;
		size_t a = rand()%4;
		if(a == 0) {
			Pong[i].ball_speed.x = 1;
			Pong[i].ball_speed.y = 1;
			}
		if(a == 1) {
			Pong[i].ball_speed.x = 1;
			Pong[i].ball_speed.y = -1;
			}
		if(a == 2) {
			Pong[i].ball_speed.x = -1;
			Pong[i].ball_speed.y =  1;
			}
		if(a == 3) {
			Pong[i].ball_speed.x = -1;
			Pong[i].ball_speed.x = -1;
			}

		}

	}

void Update_Ball(PONG *p) {

	for(size_t i = 0; i < NUMBER_OF_SPICES ; i++) {
		if(p[i].is_allive) {
			p[i].ball.x += p[i].ball_speed.x;
			p[i].ball.y += p[i].ball_speed.y;
			if(p[i].ball.y <= 0 || p[i].ball.y >= height) {
				p[i].ball_speed.y *=-1;
				}

			}
		}

	}

int rand_1Or1(void) {
	if(rand()%1==0) {
		return -1;
		}
	return 1;
	}
int rand_0Or1(void) {
	if(rand()%3==0) {
		return -1;
		}
	if(rand()%3==0)
		return 1;

	return 0;
	}


void Check_Colision_Pack(PONG *p) {

	for(size_t i = 0; i < NUMBER_OF_SPICES ; i++) {
		if(p[i].is_allive) {
			if(p[i].ball.x <= 10) {   //Left Pack
				if((p[i].ball.y >= p[i].left.y)&& (p[i].ball.y <= p[i].left.y + p[i].left.h) ) {
					//p[i].ball_speed.y *=-1;
					p[i].ball_speed.x *= -1;
					p[i].ball_speed.y = rand_0Or1();
					//population[i].fitnes -= 10000; //If Hit Pack Decress
					}
				else {
					p[i].ball.x = width  / 2;
					p[i].ball.y = height / 2;
					p[i].ball_speed.x = rand_1Or1();
					p[i].ball_speed.y = rand_0Or1();
					//population[i].fitnes += 10000; //If No Hit Pack Decress
					p[i].is_allive = 0;
					curently_alive--;
					}
				//system("pause");
				}
			if(p[i].ball.x >= width - 20) {   //Right Pack
				if((p[i].ball.y >= p[i].right.y) && (p[i].ball.y <= p[i].right.y + p[i].right.h)) {
					//p[i].ball_speed.y *=-1;
					p[i].ball_speed.x *=-1;
					p[i].ball_speed.y = rand_0Or1();
					population[i].fitnes -= 10000; //If Hit Pack Decress
					}
				else {
					p[i].ball.x = width  / 2;
					p[i].ball.y = height / 2;
					p[i].ball_speed.x = rand_1Or1();
					p[i].ball_speed.y = rand_0Or1();
					//population[i].fitnes += 10000; //If No Hit Pack Decress
					p[i].is_allive = 0;
					curently_alive--;
					}
				//system("pause");
				}
			}
		}
	}
void Update_Pack(PONG *p) {

	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) { // Update Left
		if(p[i].is_allive) {
			population[i].input.elem[0] = (float)p[i].left.y;
			population[i].input.elem[1] = (float)p[i].left.x;
			population[i].input.elem[2] = (float)p[i].ball.x;
			population[i].input.elem[3] = (float)p[i].ball.y;
			population[i].input.elem[4] = p[i].ball_speed.y;
			population[i].input.elem[5] = p[i].ball_speed.x;
			//population[i].input.elem[6] = sqrt((p[i].left.y- p[i].ball.y)*(p[i].left.y- p[i].ball.y)+(p[i].left.x- p[i].ball.x)*(p[i].left.x - p[i].ball.x));

			}

		neat_forward(population,NUMBER_OF_SPICES);
		for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
			if(population[i].out[NUMBER_OF_LAYER - 1].elem[0] > population[i].out[NUMBER_OF_LAYER - 1].elem[1]) {
				if(p[i].left.y <= height - p[i].left.h) {
					p[i].left.y += 1;
					}
				}

			else {
				if(p[i].left.y >= 0) {
					p[i].left.y -= 1;
					}

				}
			}
		}
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) { // Update Right
		if(p[i].is_allive) {
			population[i].input.elem[0] = (float)p[i].right.y;
			population[i].input.elem[1] = (float)p[i].right.x;
			population[i].input.elem[2] = (float)p[i].ball.x;
			population[i].input.elem[3] = (float)p[i].ball.y;
			population[i].input.elem[4] = p[i].ball_speed.y;
			population[i].input.elem[5] = p[i].ball_speed.x;
			//population[i].input.elem[6] = sqrt(p[i].right.y*p[i].right.y+p[i].right.x*p[i].right.x);
		  //population[i].input.elem[6] = sqrt((p[i].right.y- p[i].ball.y)*(p[i].right.y- p[i].ball.y)+(p[i].right.x- p[i].ball.x)*(p[i].right.x - p[i].ball.x));
			}

		neat_forward(population,NUMBER_OF_SPICES);
		for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
			if(population[i].out[NUMBER_OF_LAYER - 1].elem[0] > population[i].out[NUMBER_OF_LAYER - 1].elem[1]) {
				if(p[i].right.y <= height - p[i].right.h) {
					p[i].right.y += 1;
					}
				}

			else {
				if(p[i].right.y >= 0) {
					p[i].right.y -= 1;
					}

				}
			}
		}

	}


void Neat_Reset_Fitnes() {
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		population[i].fitnes = 100000.0f;
		//po
		}

	}

void Main_Renderer(SDL_Renderer *r, PONG *p,int Ndisplay) {
	SDL_RenderClear(r);
	for(size_t i = 0; i < Ndisplay; i++) {

		if(p[i].is_allive==1) {
			SDL_SetRenderDrawColor(r,p[i].color.r,p[i].color.g,p[i].color.b,255);
			SDL_RenderFillRect(r,&p[i].left);
			SDL_RenderFillRect(r,&p[i].right);
			SDL_RenderFillRect(r,&p[i].ball);
			}


		}
	SDL_SetRenderDrawColor(r,0,23,10,255);
	SDL_RenderPresent(r);

	}

int main(void) {

	SDL_Renderer *renderer;
	SDL_Window 	 *window;
	SDL_CreateWindowAndRenderer(width,height,SDL_RENDERER_ACCELERATED,&window,&renderer);
	PONG Pong[NUMBER_OF_SPICES];
	srand(9999);
	Pong_Init(Pong);
	neat_alloc(population);
	printf("Do you want to load model(y - YES)\n>");
	char choice,name[30];	
	int Niter;
	char choice1;
	scanf("%c",&choice);
	if(choice == 'y'){
		printf("Name of model\n>");
		scanf("%s",name);
		neat_load(population,name);	
		choice = 'n';
	}
	else{
		neat_rand(population,-3,3);	
	}
	//system("pause");
	fflush(stdin);
	printf("Do you want to save model(y - YES)\n>");
	scanf("%c",&choice1);
	if(choice1 == 'y'){
		printf("How mutch iteration till save\n>");
		scanf("%d",&Niter);
		printf("Name of saved file\n>");
		scanf("%s",name);
		
	}
	Neat_Reset_Fitnes();
	size_t counter = 1;

	while(1) {

		SDL_Event event;
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT){
				return 0;
			}
			}
			Main_Renderer(renderer,Pong,10);
			
			//SDL_Delay(10);
		

		Check_Colision_Pack(Pong);
		Update_Ball(Pong);
		Update_Pack(Pong);
		if(curently_alive == 0) {
			counter++;
			for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
				Pong[i].is_allive = 1;
				}
			curently_alive = NUMBER_OF_SPICES;
			neat_crossover(population);
			printf("BEST FITNES IS %f\n",population[0].fitnes);
			Neat_Reset_Fitnes();
			if((counter % (Niter + 1) == 0) && (choice1 == 'y')){
				neat_save(population,name);
			
			}
			}


		}

	system("pause");
	return 0;
	}
