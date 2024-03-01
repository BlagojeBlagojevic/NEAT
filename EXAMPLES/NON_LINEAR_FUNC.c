#define NUMBER_OF_SPICES 100
#define LR 0.1
#define MUTATION_RATE 0.2
#define NUMBER_OF_LAYER 2
#define NUMBER_OF_NEURON 10
#define NINPUTS 1
#define NOUT 1
#define NEAT_IMPLEMETATION
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_TANH
#define SEED 10
#include "neat.h"
#include<math.h>
int main() {

	static NEAT n[NUMBER_OF_SPICES + 1];  //MACRO EXTENDED
	//system("pause");
	//static NEAT n1[NUMBER_OF_SPICES];
	neat_alloc(n);
	//neat_alloc(n1);
	neat_rand(n,-2.0f,2.0f);
	srand(SEED);
	  
 float error = 0.0f;
  for(size_t counter = 0;counter < 100;counter++){
  	for(size_t j = 0; j < NUMBER_OF_SPICES;j++){
  		 error = 0.0f;
			for(float i = -20;i < 20;i+=0.1){
				//float a = (float)(rand()%10);
  			float out = (float)sin(i/(2*3.14f));
  			n[j].input.elem[0] = (float)i;
  			neat_forward(&n[j],1);
  			error+=((out) - n[j].out[NUMBER_OF_LAYER - 1].elem[0])*((out) - n[j].out[NUMBER_OF_LAYER - 1].elem[0]);
				//error+=fabs(out - n[j].out[NUMBER_OF_LAYER - 1].elem[0]);
				//n[j].fitnes = sqrt(error) / 400.0f;
					
			}
		n[j].fitnes = sqrt(error) / 40.0f;
		neat_crossover(n);
		error = 0.0f;
		}
		//neat_crossover(n);
		printf("best fit GEN %d is %f\n",counter,n[0].fitnes);
	}
	while(1){
		printf("Input value(x, y):\t");
		float x,y;
		scanf("%f",&x);
		//system("pause");
		for(size_t k = 0;k < NUMBER_OF_SPICES;k++){
		n[k].input.elem[0] = x;
		//n[k].input.elem[1] = y;
		}
			//n[k].input.elem[0] = x;
		//system("pause");
		neat_forward(&n[0],1);
		matrix_print_out(n[0].out[NUMBER_OF_LAYER - 1],"OUT ",NOUT-1);
		printf("\ncoreect(sin(%f/(3.14*2))) = %f\n",x,sin(x/(2*3.14f)));
		//neat_print(&n[0]);
	}
}
