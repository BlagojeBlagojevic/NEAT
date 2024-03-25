#define NUMBER_OF_SPICES 20
#define NUMBER_OF_SPICES_IN_CROSOWER 2
#define LR 0.005
#define MUTATION_RATE 0.1
#define NUMBER_OF_LAYER 2
#define NUMBER_OF_NEURON 10
#define NINPUTS 764 / 3
#define NOUT 10
#define NEAT_IMPLEMETATION
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_SWISH
#define ENABLE_MUTATION 0.2
#define DISABLE_MUTATION 0.04
//#define NEAT_IMPLEMETATION
#define ENABLE_SOFTMAX
#include "neat.h"
#include<string.h>
#define NUM_OF_IMAGES 9*10

#include<float.h>
#define IMAGE_IMPLEMENTATION
#include "image.h"


int main() {

	//Mat compresion = matrix_alloc(28*28,28);
	NEAT population[NUMBER_OF_SPICES + 1];
	Mat image[NUM_OF_IMAGES];
	Image ia;
	int label[NUM_OF_IMAGES];
	char path[] = "mnist/0/1.png";
	//uint8_t *pixels;
	//int width,height,chanels;
	//pixels = (uint8_t *)calloc(28*28*2,sizeof(uint8_t));
	
	//FILE *f;
	neat_alloc(population);
	//neat_load(population,"mnist.bin");
	//neat_rand(population,-3,3);
	//neat_load(population,"mnist.bin");
	for(size_t i = 0; i < NUM_OF_IMAGES; i++) {
		image[i] = matrix_alloc(1,NINPUTS);

		}
	//srand(time(0));
	srand(9999);
	neat_alloc(population);
	for(size_t i = 0,counter = 1; i < NUM_OF_IMAGES - 1; i++) {
		

		//if(f==NULL)
		label[i] = path[6] - 48;
		printf("%s\n",path);
		path[8] = '0' + counter++;
		if(counter == 10) {

			counter = 1;
			path[6]++;

			}
		 ia = Image_Alloc_Name(path);
		 //Image_Applay_Kernel(ia,ia,KERNEL_EDGE,3,3);
		 //Image_Black_White_Filtar(ia,150);
		 printf("Size %d\n",ia.chanels*ia.height*ia.width);
			for(size_t j = 0; j < 764 ; j+=3) {
				image[i].elem[j] = (float)ia.pixels[j] / 255.0f;
			}
			Image_Free(ia);
		//fclose(f);
		//matrix_dot(out[i],image[i],compresion);
		}



	for(size_t z = 0; z < NUM_OF_IMAGES  - 1; z++) {
		printf("label(%d) %d\n",z,label[z]);
		}
	system("pause");
	// Training loop
	for(size_t i = 0; i < 10000; i++) {
		
		for(size_t z = 0; z < NUM_OF_IMAGES  - 1; z++) {
			for(size_t x = 0; x < NUMBER_OF_SPICES; x++) {
				for(size_t y = 0; y < NINPUTS && z%9!=0; y++)
					population[x].input.elem[y] = image[z].elem[y];
				}
			neat_forward(population,NUMBER_OF_SPICES);
			for(size_t x = 0; x < NUMBER_OF_SPICES; x++) {
				//population[x].fitnes--;
				//population[x].fitnes-=population[x].out_softmax.elem[label[z]];
				if(population[x].out_softmax.elem[label[z]] > 0.7f)
					population[x].fitnes-=population[x].out_softmax.elem[label[z]];

				}
				//neat_crossover(population);
			}
		neat_crossover(population);
		printf("Fitnes(%I64u)  is %f\n",i,population[0].fitnes);
		///////printf("Corect images %f\n" ,(float) (population[0].fitnes - 81) / (float)(99932) 
		Neat_Reset_Fitnes(population);


		}
	neat_save(population,"mnist.bin");
	while(1) {
		printf("what image to  test:\n");
		int a;
		scanf("%d",&a);
		for(size_t i = 0; i < NINPUTS; i++) {
			population[0].input.elem[i] = image[a].elem[i];
			}
		neat_forward(population,1);
		//MATRIX_PRINT(population[0].out[NUMBER_OF_LAYER - 1]);
		//matrix_mul_scalar(population[0].out_softmax, 100);
		MATRIX_PRINT(population[0].out_softmax);
		//system("pause");
		}


	return 0;
	}
