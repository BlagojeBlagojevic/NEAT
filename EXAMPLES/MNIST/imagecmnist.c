#define NUMBER_OF_SPICES 20
#define NUMBER_OF_SPICES_IN_CROSOWER 5
#define LR 0.2
#define MUTATION_RATE 0.2
#define NUMBER_OF_LAYER 2
#define NUMBER_OF_NEURON 10
#define NINPUTS 28*28
#define NOUT 10
#define NEAT_IMPLEMETATION
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_RELU
#define ENABLE_MUTATION 0.2
#define DISABLE_MUTATION 0.01
#define NEAT_IMPLEMETATION
#define ENABLE_SOFTMAX
#include "neat.h"
#include<string.h>
#define NUM_OF_IMAGES 7*3

#include<float.h>

NEAT population[NUMBER_OF_SPICES + 1];

int main() {

	//Mat compresion = matrix_alloc(28*28,28);
	Mat image[NUM_OF_IMAGES];
	int label[NUM_OF_IMAGES];
	char path[] = "mnist/0/1.png";
	FILE *f;
	neat_alloc(population);
	//neat_rand(population,-3,3);
	for(size_t i = 0; i < NUM_OF_IMAGES; i++) {
		image[i] = matrix_alloc(1,28*28);

		}
	srand(time(0));
	neat_alloc(population);
	for(size_t i = 0,counter = 1; i < NUM_OF_IMAGES - 1; i++) {


		//if(f==NULL)
		label[i] = path[6] - 48;
		printf("%s\n",path);
		path[8] = '0' + counter++;
		if(counter == 8) {

			counter = 1;
			path[6]++;

			}
		f = fopen(path,"rb");
		if(f == NULL)
			exit(1);

		for(size_t j = 0; j < 28*28; j++) {
			image[i].elem[j] = (float)fgetc(f) / 255  ;
			}

		fclose(f);
		//matrix_dot(out[i],image[i],compresion);
		}



	for(size_t z = 0; z < NUM_OF_IMAGES  - 1; z++) {
		printf("label %d\n",label[z]);
		}
	system("pause");
	// Training loop
	for(size_t i = 0; i < 1000; i++) {
		Neat_Reset_Fitnes(population);
		for(size_t z = 0; z < NUM_OF_IMAGES  - 1; z++) {
			for(size_t x = 0; x < NUMBER_OF_SPICES; x++) {
				for(size_t y = 0; y < NINPUTS; y++)
					population[x].input.elem[y] = image[z].elem[y];
				}
			neat_forward(population,NUMBER_OF_SPICES);
			for(size_t x = 0; x < NUMBER_OF_SPICES; x++) {
				if(population[x].out[NUMBER_OF_LAYER - 1].elem[label[z]] > 0.7f)
					population[x].fitnes-=population[x].out_softmax.elem[label[z]];
				//for(size_t k = 0;k < 10;k++){
				//if(population[x].out[NUMBER_OF_LAYER - 1].elem[k] > 0.2f && k!=label[z]){
				//	population[x].fitnes++;
				//	}
				//}
				}

			}
		neat_crossover(population);
		printf("Fitnes  is %f\n", population[0].fitnes);


		}

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
