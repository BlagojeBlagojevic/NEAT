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

#ifndef NOUT 
#define NOUT 1
#endif

#ifndef PROB_OF_BEST
#define  PROB_OF_BEST 0.1
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
	
}NEAT;

void neat_alloc(NEAT *n);
void neat_free(NEAT *n);
void neat_rand(NEAT *n, float low, float high);
void neat_forward(NEAT *n,size_t Niter);
void neat_print(NEAT *n);
void neat_crossover(NEAT *n, NEAT *n1);
void neat_mutation(NEAT *n);
void neat_reproduce(NEAT *n1,NEAT *n2);


#endif

#ifdef NEAT_IMPLEMETATION
void neat_alloc(NEAT *n){
	
	  	//*n = (NEAT*)calloc(NUMBER_OF_SPICES, sizeof(NEAT*));
	  	for(size_t i = 0; i < NUMBER_OF_SPICES; i++){
	  		n[i].fitnes = 10;
	  		n[i].input       = matrix_alloc(1,NINPUTS);
	  		n[i].weigts[0]   = matrix_alloc(NINPUTS,NUMBER_OF_NEURON);
			  n[i].bias[0]	   = matrix_alloc(1,NUMBER_OF_NEURON);
 	      n[i].out[0]      = matrix_alloc(1,NUMBER_OF_NEURON);
     	  n[i].out_softmax = matrix_alloc(1,NOUT);
		
	  		
	  		for(size_t j = 1; j < NUMBER_OF_LAYER;j++){
            n[i].weigts[j]  = matrix_alloc(NUMBER_OF_NEURON,NUMBER_OF_NEURON);
	  				n[i].bias[j]    = matrix_alloc(1,NUMBER_OF_NEURON);
	  				n[i].out[j]     = matrix_alloc(1,NUMBER_OF_NEURON);
			  }
             //Widjeti kako
	  		//n[i].weigts[NUMBER_OF_LAYER - 1]  = matrix_alloc(NOUT,NUMBER_OF_NEURON);
		    //n[i].bias[NUMBER_OF_LAYER - 1]    = matrix_alloc(1,NOUT);
		    //n[i].out[NUMBER_OF_LAYER - 1]     = matrix_alloc(1,NOUT);
		  }
	
}
void neat_free(NEAT *n){
	
	for(size_t i = 0;i < NUMBER_OF_SPICES;i++){
	matrix_free(n[i].input);
	//system("pause");
	for(size_t j = 0;j < NUMBER_OF_LAYER;j++){
			matrix_free(n[i].weigts[j]);
		//	system("pause");
			matrix_free(n[i].bias[j]);
			//system("pause");
			matrix_free(n[i].out[j]);
			//system("pause");
		
	}	
	}
	
 
}


void neat_rand(NEAT *n, float low, float high){
	
	for(size_t i = 0;i < NUMBER_OF_SPICES; i++){
	 for(size_t j = 0; j < NUMBER_OF_LAYER;j++){
	 	     matrix_rand(n[i].bias[j],low,high);	
     		 matrix_rand(n[i].weigts[j],low,high);
	 }

	}
}

void neat_forward(NEAT *n,size_t Niter){
	
	
	for(size_t i = 0;i < Niter; i++){
		//system("pause");
		matrix_feedforward(n[i].out,n[i].input,n[i].weigts,n[i].bias,NUMBER_OF_LAYER);
		matrix_copy(n[i].out_softmax,n[i].out[NUMBER_OF_LAYER-1]);
		matrix_softmax(n[i].out_softmax);
		//matrix_print_out(n[i].out_softmax,"out_SOFTMAX",NOUT);
		//matrix_print_out(n[i].out[NUMBER_OF_LAYER - 1],"OUT",NOUT);
		//system("pause");
	}
}

void neat_print(NEAT *n){
	
	MATRIX_PRINT(n[0].input);
	for(size_t j = 0; j < NUMBER_OF_SPICES;j++){
		system("pause");
		printf("\n");
		system("cls");
		printf("\t\t\t NEAT SPICES %d\n",j);
		printf("\n\n______________________________________________________________________________\n");
	  system("pause");
	  
		for(size_t i = 0; i < NUMBER_OF_LAYER;i++){
		
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

void neat_mutation(NEAT *n){
	for(size_t i = 0; i < NUMBER_OF_SPICES;i++){
	 for(size_t j = 0; j < NUMBER_OF_LAYER;j++){
	 	 matrix_mutation(n[i].weigts[j]);
	 	 matrix_mutation(n[i].bias[j]);
	 }
		
	}
}

void neat_reproduce(NEAT *n1,NEAT *n2){
	for(size_t i = 0;i < NUMBER_OF_LAYER;i++){
			matrix_reproduce(n1[0].weigts[i],n2[0].weigts[i]);
			matrix_reproduce(n1[0].bias[i],n2[0].bias[i]);
	}

}



void neat_crossover(NEAT *n, NEAT *n1){
			
		 for(size_t i = 0;i < NUMBER_OF_SPICES;i++){
		 	for(size_t j = i + 1;j < NUMBER_OF_SPICES;j++){
 		 		if(n[j].fitnes < n[i].fitnes){
		 			 memcpy(&n1[0],&n[i],sizeof(n[i]));
		 			 memcpy(&n[i],&n[j],sizeof(n[j]));
		 			 memcpy(&n[j],&n1[0],sizeof(n1[0]));
				 }
			 }}
			 //printf("\n\nMIN = %f",n[0].fitnes);
			memcpy(&n1[0],&n[0],sizeof(n[0]));
		 	for(size_t i = 1;i < NUMBER_OF_SPICES;i++){
		 		
				if(rand_float() < PROB_OF_BEST)
				neat_reproduce(&n[i],&n[0]);
				else{
					int s2 = rand()%(NUMBER_OF_SPICES / 10);
		 					neat_reproduce(&n[i],&n[s2]);
				}
		 		
		 		//neat_reproduce(&n[s2],&n[0]);
			 }
		  //memcpy(&n[NUMBER_OF_SPICES - 1],&n1[0],sizeof(n1[0]));
	    	 	
		 	
		 }
	
#endif

