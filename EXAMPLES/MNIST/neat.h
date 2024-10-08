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

extern inline void neat_alloc(NEAT *__restrict__ n);
extern inline void neat_free(NEAT *__restrict__ n);
extern inline void neat_rand(NEAT *__restrict__ n, float low, float high);
extern inline void neat_forward(NEAT *__restrict__ n,size_t Niter);
extern inline void neat_print(NEAT *__restrict__ n);
extern inline void neat_crossover(NEAT *__restrict__ n);
extern inline void neat_mutation(NEAT *__restrict__ n);
extern inline void neat_reproduce(NEAT *__restrict__ n1,NEAT *__restrict__ n2);
extern inline void neat_save(NEAT *__restrict__ n,const char *name);
extern inline void neat_load(NEAT *__restrict__ n,const char *name);
extern inline void Neat_Reset_Fitnes(NEAT *n);

#endif

#ifdef NEAT_IMPLEMETATION
extern inline void neat_alloc(NEAT *__restrict__ n) {

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
extern inline void neat_free(NEAT  *__restrict__ n) {

	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
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


extern inline void neat_rand(NEAT *__restrict__ n, float low, float high) {

	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			matrix_rand(n[i].bias[j],low,high);
			matrix_rand(n[i].weigts[j],low,high);
			}

		}
	}

extern inline void neat_forward(NEAT  *__restrict__ n,size_t Niter) {


	for(size_t i = 0; i < Niter; i++) {
		//system("pause");
		matrix_feedforward(n[i].out,n[i].input,n[i].weigts,n[i].bias,NUMBER_OF_LAYER);
		matrix_copy(n[i].out_softmax,n[i].out[NUMBER_OF_LAYER-1]);
#ifdef ENABLE_SOFTMAX
		matrix_softmax(n[i].out_softmax);
#endif
		//matrix_print_out(n[i].out_softmax,"out_SOFTMAX",NOUT);
		//matrix_print_out(n[i].out[NUMBER_OF_LAYER - 1],"OUT",NOUT);
		//system("pause");
		}
	}

extern inline void neat_print(NEAT *__restrict__ n) {

	MATRIX_PRINT(n[0].input);
	for(size_t j = 0; j < NUMBER_OF_SPICES; j++) {
		system("pause");
		printf("\n");
		system("cls");
		printf("\t\t\t NEAT SPICES %d\n",(int)j);
		printf("\n\n______________________________________________________________________________\n");
		system("pause");

		for(size_t i = 0; i < NUMBER_OF_LAYER; i++) {

			printf("\ni =  %d \n\nj =  %d",(int)i,(int)j);
			MATRIX_PRINT(n[j].weigts[i]);
			printf("\ni =  %d \n\nj =  %d",(int)i,(int)j);
			MATRIX_PRINT(n[j].bias[i]);
			printf("\ni =  %d \n\nj =  %d",(int)i,(int)j);
			MATRIX_PRINT(n[j].out[i]);
			printf("\ni =  %d \n\nj =  %d",(int)i,(int)j);
			}
		MATRIX_PRINT(n[j].out_softmax);
		printf("\nj =  %d",(int)j);
		}
	}

extern inline void neat_mutation(NEAT *__restrict__ n) {
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		for(size_t j = 0; j < NUMBER_OF_LAYER; j++) {
			matrix_mutation(n[i].weigts[j]);
			matrix_mutation(n[i].bias[j]);
			}

		}
	}
extern inline void neat_reproduce(NEAT *__restrict__  n1,NEAT *__restrict__ n2) {
	for(size_t i = 0; i < NUMBER_OF_LAYER; i++) {
		matrix_reproduce(n1[0].weigts[i],n2[0].weigts[i]);
		matrix_reproduce(n1[0].bias[i],n2[0].bias[i]);
		}

	}



extern inline void neat_crossover(NEAT *__restrict__ n) {

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

extern inline void Neat_Reset_Fitnes(NEAT  *__restrict__ n) {
	for(size_t i = 0; i < NUMBER_OF_SPICES; i++) {
		n[i].fitnes = 100000.0f;
		//po
		}

	}

extern inline void neat_save(NEAT *__restrict__ n,const char *name) {
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


extern inline void neat_load(NEAT *__restrict__ n,const char *name) {
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
