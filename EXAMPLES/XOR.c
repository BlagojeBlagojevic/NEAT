#define NUMBER_OF_SPICES 5000
#define LR 0.6
#define MUTATION_RATE 0.9
#define NUMBER_OF_LAYER 2
#define NUMBER_OF_NEURON 2
#define NINPUTS 2
#define NOUT 1
#define NEAT_IMPLEMETATION
#include "neat.h"

int main(){

	static NEAT n[NUMBER_OF_SPICES];  //MACRO EXTENDED
	//system("pause");
	static NEAT n1[NUMBER_OF_SPICES];
	neat_alloc(n);
	neat_alloc(n1);
	
	srand(time(0));
	neat_rand(n,-30,30);
	//Mat xor_model;
	size_t counter = 0;
	for(size_t i = 0; i < 10000;counter++,i++){
		//int a = rand()%2;
		//int b = rand()%2;
		//int out = a^b;
		//printf("%d xor %d = %d\n",a,b,out);
		for(size_t j = 0;j < NUMBER_OF_SPICES;j++)
		{
		  float err = 0;
	    for(int y = 0;y < 2;y++){
	    	for(int x = 0;x < 2;x++){
	    	 n[j].input.elem[0] = x;
				 n[j].input.elem[1] = y;				 
				 uint32_t out = (x^y);
				 
	    	 neat_forward(&n[j],1);	
	    	 err += ((float)out - n[j].out[NUMBER_OF_LAYER - 1].elem[0])
			* ((float)out - n[j].out[NUMBER_OF_LAYER - 1].elem[0]); 
				 						//printf("out %d, %f\n",out, err);
				}
			}
			
			//system("pause");
			n[j].fitnes = sqrt((err))*400;
		}
	
		neat_crossover(n,n1);
		
		printf("GEN %lld fitnes = %f\n ",counter,n[0].fitnes);
		
		if(n[0].fitnes < 0.01 && (i>10)){
			i = 10000;
			break;
		}
		for(size_t k = 0; k < NUMBER_OF_SPICES;k++)
		{
		//n[k].fitnes = 0.0f;
	}
		
		//system("pause");
	}
	//MATRIX_PRINT(n[0].input);
	//MATRIX_PRINT(n[0].out[NUMBER_OF_LAYER - 1]);
	while(1){
		int a,b;
		printf("Unesite A,B : \n");
		scanf("%d %d",&a,&b);
		//int out = a^b;
		//float err = ((float)out - n[0].out[NUMBER_OF_LAYER - 1].elem[0]);
		for(size_t j = 0;j < NUMBER_OF_SPICES;j++)
		{
			n[j].input.elem[0] = (float)a;
			n[j].input.elem[1] = (float)b;
			
		}
		neat_forward(&n[0],1);
		//neat_print(&n[0]);
		matrix_print_out(n[0].out[NUMBER_OF_LAYER - 1],"OUT ",NOUT-1);
		//neat_print(&n[0]);
	}
	MATRIX_PRINT(n[0].weigts[0]);
	MATRIX_PRINT(n[0].weigts[1]);
	//MATRIX_PRINT(n[0].bias[0]);
  //MATRIX_PRINT(n[0].bias[1]);
	//neat_print(n);
	
	return 0;
}
