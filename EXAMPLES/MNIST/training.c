#define NUMBER_OF_SPICES 20
#define NUMBER_OF_SPICES_IN_CROSOWER 2
#define LR 0.05
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
#define T 1
//#define NEAT_IMPLEMETATION
#define ENABLE_SOFTMAX
#include "neat.h"
#include<string.h>
#define NUM_OF_IMAGES 4000*10
#include<float.h>
#define IMAGE_IMPLEMENTATION
#include "image.h"

#define NUM_OF_FILES 10000
#define MAX_SIZE_STR 100 

#include <dirent.h>

#define LOG 0
size_t load_files_from_dir(const char* name, char path[NUM_OF_FILES][MAX_SIZE_STR]) {
	DIR *d = opendir(name);
	struct dirent *dir;    //Intermidiead object
	if(d == NULL)
		assert(0 && "NON EXISTENT FILE !!!");
	size_t counter = 0;
	while((dir = readdir(d)) != NULL) {

		strcpy(path[counter++],dir->d_name);
		char pom[20];
		strcpy(pom,name);
		strcat(pom,path[counter - 1]);
		strcpy(path[counter - 1],pom);
#if LOG == 1
		printf("%s\n",path[counter - 1]);
#endif
		assert(counter <= NUM_OF_FILES);
		}
	closedir(d);
	return counter;
	}






int main() {

	
	//Mat compresion = matrix_alloc(28*28,28);
	static NEAT population[NUMBER_OF_SPICES + 1];
	static Mat image[NUM_OF_IMAGES];
	Image ia;
	static int label[NUM_OF_IMAGES];
	int counter = 0;
	
	//char name[] = "mnist/0/";
	char name[] = "mnist1/0/";
	neat_alloc(population);
	neat_load(population, "a.bin");

	//neat_rand(population, -1,1);
	for(size_t i = 0; i < NUM_OF_IMAGES; i++) {
		image[i] = matrix_alloc(1,NINPUTS);

		}


	for (size_t i = 0; i < 10; i++)
	{
		
		char path[NUM_OF_FILES][MAX_SIZE_STR];
		int n_images = load_files_from_dir(name,path);
		printf("%d", n_images);
		for (size_t j = 0; j < n_images; j++)
		{
			label[counter] = (int)i;
			//DEFAULT PATH
			if (path[j][9] != '.')
			{
				Image ia = Image_Alloc_Name(path[j]);
				Image_Black_White_Filtar(ia,125);
				//printf("%s\n",path[j]);
				//Image_Save(ia,"saved.jpg");
				//return 0;
			
			
			//PIXELSS
			for(size_t z = 0; z < ia.height*ia.width ; z++) {
				//printf("z %d\n",z);
				image[counter].elem[z] = (float)ia.pixels[z] / 255.0f;
			}
			//printf("Nesto %d", j);
			counter++;
			Image_Free(ia);	
			}
		}	
		
		name[7]++;

	}
	

// Training loop

	for(size_t i = 0; i < 100; i++) {
		printf("Training_loop\n ");
		for(size_t z = 0; z < NUM_OF_IMAGES  - 1; z++) {
			for(size_t x = 0; x < NUMBER_OF_SPICES; x++) {
				for(size_t y = 0; y < NINPUTS; y++)
					population[x].input.elem[y] = image[z].elem[y];
				}
			neat_forward(population,NUMBER_OF_SPICES);
			for(size_t x = 0; x < NUMBER_OF_SPICES; x++) {
				//population[x].fitnes--;
				//population[x].fitnes-=population[x].out_softmax.elem[label[z]];
				if(population[x].out_softmax.elem[label[z]] > 0.5f)
					population[x].fitnes-=population[x].out_softmax.elem[label[z]];

				}
				//neat_crossover(population);
			}
		neat_crossover(population);
		printf("Fitnes(%d)  is %f\n",(int)i,population[0].fitnes);
		///////printf("Corect images %f\n" ,(float) (population[0].fitnes - 81) / (float)(99932) 
		Neat_Reset_Fitnes(population);


		}




	neat_save(population,"a.bin");
	printf("Save\n");
	while(1) {
		printf("what image to  test:\n");
		int a;
		scanf("%d",&a);
		printf("Before");
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
