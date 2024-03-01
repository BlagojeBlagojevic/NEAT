#ifndef NEAT_H
#define NEAT_H
#define MATRIX_IMPLEMETATION
//#define TYPE int
#include "matrix.h"
#include<time.h>

#ifndef NUMBER_OF_SPICES
#define NUMBER_OF_SPICES 100
#endif

#ifndef NUMBER_OF_LAYER
#define NUMBER_OF_LAYER 4
#endif

#ifndef NUMBER_OF_NEURON
#define NUMBER_OF_NEURON 2
#endif

#ifndef NINPUTS
#define NINPUTS 2
#endif

#ifndef NUMBER_OF_SPICES_IN_CROSOWER
#define NUMBER_OF_SPICES_IN_CROSOWER 5
#endif

#ifndef NOUT
#define NOUT 1
#endif
//#define MUTATION_RATE 0.1
#define BETA 0.1
//#define NUMBER_OF_SPICES_IN_CROSOWER 8

typedef struct {

	float fitnes;
	Mat input;
	Mat weigts[NUMBER_OF_LAYER];
	Mat bias[NUMBER_OF_LAYER];
	Mat out[NUMBER_OF_LAYER];
	Mat out_softmax;
	} NEAT;

static inline void neat_alloc(NEAT *n);
static inline void neat_free(NEAT *n);
static inline void neat_rand(NEAT *n, float low, float high);
static inline void neat_forward(NEAT *n,size_t Niter);
static inline void neat_print(NEAT *n);
static inline void neat_crossover(NEAT *n);
static inline void neat_mutation(NEAT *n);
static inline void neat_reproduce(NEAT *n1,NEAT *n2);
static inline void neat_save(NEAT *n,const char *name);
static inline void neat_load(NEAT *n,const char *name);

#endif

#ifdef NEAT_IMPLEMETATION
static inline void neat_alloc(NEAT *n) {

	//*n = (NEAT*)calloc(NUMBER_OF_SPICES, sizeof(NEAT*));
	for(size_t i = 0; i <= NUMBER_OF_SPICES; i++) {
		n[i].input       = matrix_alloc(1,NINPUTS);
		n[i].weigts[0]   = matrix_alloc(NINPUTS,NUMBER_OF_NEURON);
		n[i].bias[0]	   = matrix_alloc(1,NUMBER_OF_NEURON);
		n[i].out[0]      = matrix_alloc(1,NUMBER_OF_NEURON);
		n[i].out_softmax = matrix_alloc(1,NOUT);


		for(size_t j = 1; j < NUMBER_OF_LAYER - 1; j++) {
			n[i].weigts[j]  = matrix_alloc(NUMBER_OF_NEURON,NUMBER_OF_NEURON);
			n[i].bias[j]    = matrix_alloc(1,NUMBER_OF_NEURON);
			n[i].out[j]     = matrix_alloc(1,NUMBER_OF_NEURON);
			}
		//Widjeti kako
		n[i].weigts[NUMBER_OF_LAYER - 1]  = matrix_alloc(NUMBER_OF_NEURON,NOUT);
		n[i].bias[NUMBER_OF_LAYER - 1]    = matrix_alloc(1,NOUT);
		n[i].out[NUMBER_OF_LAYER - 1]     = matrix_alloc(1,NOUT);
		}

	}
static inline void neat_free(NEAT *n) {

	for(size_t i = 0; i <= NUMBER_OF_SPICES; i++) {
		matrix_free(n[i].input);
		//system("pause");
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			matrix_free(n[i].weigts[j]);
			//	system("pause");
			matrix_free(n[i].bias[j]);
			//system("pause");
			matrix_free(n[i].out[j]);
			//system("pause");

			}
		}


	}


static inline void neat_rand(NEAT *n, float low, float high) {

	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			matrix_rand(n[i].bias[j],low,high);
			matrix_rand(n[i].weigts[j],low,high);
			}

		}
	}

static inline void neat_forward(NEAT *n,size_t Niter) {


	for(size_t i = 0; i < Niter; i++) {
		//system("pause");
		matrix_feedforward(n[i].out,n[i].input,n[i].weigts,n[i].bias,NUMBER_OF_LAYER);
		matrix_copy(n[i].out_softmax,n[i].out[NUMBER_OF_LAYER-1]);
		//matrix_softmax(n[i].out_softmax);
		//matrix_print_out(n[i].out_softmax,"out_SOFTMAX",NOUT);
		//matrix_print_out(n[i].out[NUMBER_OF_LAYER - 1],"OUT",NOUT);
		//system("pause");
		}
	}

static inline void neat_print(NEAT *n) {

	MATRIX_PRINT(n[0].input);
	for(size_t j = 0; j < NUMBER_OF_SPICES; j++) {
		system("pause");
		printf("\n");
		system("cls");
		printf("\t\t\t NEAT SPICES %d\n",j);
		printf("\n\n______________________________________________________________________________\n");
		system("pause");

		for(size_t i = 0; i < NUMBER_OF_LAYER; i++) {

			printf("\ni = %ld \n\nj = %ld",i,j);
			MATRIX_PRINT(n[j].weigts[i]);
			printf("\ni = %ld \n\nj = %ld",i,j);
			MATRIX_PRINT(n[j].bias[i]);
			printf("\ni = %ld \n\nj = %zu",i,j);
			MATRIX_PRINT(n[j].out[i]);
			printf("\ni = %ld \n\nj = %ld",i,j);
			}
		MATRIX_PRINT(n[j].out_softmax);
		printf("\nj = %ld",j);
		}
	}

static inline void neat_mutation(NEAT *n) {
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			matrix_mutation(n[i].weigts[j]);
			matrix_mutation(n[i].bias[j]);
			}

		}
	}

static inline void neat_reproduce(NEAT *n1,NEAT *n2) {
	for(size_t i = 0; i < NUMBER_OF_LAYER; i++) {
		matrix_reproduce(n1[0].weigts[i],n2[0].weigts[i]);
		matrix_reproduce(n1[0].bias[i],n2[0].bias[i]);
		}

	}



static inline void neat_crossover(NEAT *n) {

	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		for(size_t j = i + 1; j < NUMBER_OF_SPICES; j++) {
			if(n[j].fitnes < n[i].fitnes) {
				memcpy(&n[NUMBER_OF_SPICES - 1],&n[i],sizeof(n[i]));
				memcpy(&n[i],&n[j],sizeof(n[j]));
				memcpy(&n[j],&n[NUMBER_OF_SPICES - 1],sizeof(n[NUMBER_OF_SPICES - 1]));
				}
			}
		}

	//ELITIZAM
	if(n[0].fitnes < n[NUMBER_OF_SPICES].fitnes)
		memcpy(&n[NUMBER_OF_SPICES],&n[0],sizeof(n[0]));
	if(n[NUMBER_OF_SPICES - 1].fitnes > n[NUMBER_OF_SPICES].fitnes)
		memcpy(&n[NUMBER_OF_SPICES - 1],&n[NUMBER_OF_SPICES],sizeof(n[0]));
	//

	//printf("\n\nMIN = %f",n[0].fitnes);
	//memcpy(&n1[0],&n[0],sizeof(n[0]));
	for(size_t i = 1; i < NUMBER_OF_SPICES - 1; i++) {

		//int s1 = rand()%(NUMBER_OF_SPICES / 10);
		if(rand_float() < 0.1)
			neat_reproduce(&n[i],&n[0]);
		else {
			int s2 = rand()%(NUMBER_OF_SPICES_IN_CROSOWER);
			neat_reproduce(&n[i],&n[s2]);
			}
		}



	}

static inline void neat_save(NEAT *n,const char *name) {
	FILE *f = fopen(name,"wb");
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		fwrite(n[i].input.elem,sizeof(float),n[i].input.cols*n[i].input.rows,f);
		//system("pause");
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			fwrite(n[i].weigts[j].elem,sizeof(float),n[i].weigts[j].cols*n[i].weigts[j].rows,f);
			fwrite(n[i].bias[j].elem,sizeof(float),n[i].bias[j].cols*n[i].bias[j].rows,f);
			//fwrite(n[i].bias[j].elem,sizeof(float),n[i].bias[j].cols*n[i].bias.rows,f);
			}
		}
	fclose(f);

	}


static inline void neat_load(NEAT *n,const char *name) {
	FILE *f = fopen(name,"rb");
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		fread(n[i].input.elem,sizeof(float),n[i].input.cols*n[i].input.rows,f);
		//system("pause");
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			fread(n[i].weigts[j].elem,sizeof(float),n[i].weigts[j].cols*n[i].weigts[j].rows,f);
			fread(n[i].bias[j].elem,sizeof(float),n[i].bias[j].cols*n[i].bias[j].rows,f);
			//fwrite(n[i].bias[j].elem,sizeof(float),n[i].bias[j].cols*n[i].bias.rows,f);
			}
		}
	fclose(f);
	}

#endif
